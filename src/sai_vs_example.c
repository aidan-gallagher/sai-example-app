#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sai.h>
#include <saivs.h>

// Global flag for graceful shutdown
volatile sig_atomic_t keep_running = 1;

void signal_handler(int signum) {
    printf("\nReceived signal %d, shutting down...\n", signum);
    keep_running = 0;
}

// Service method table implementation
const char* profile_get_value(
    sai_switch_profile_id_t profile_id,
    const char* variable)
{
    printf("[CONFIG] Request for: %s\n", variable);
    
    if (strcmp(variable, SAI_KEY_VS_SWITCH_TYPE) == 0) {
        return SAI_VALUE_SAI_SWITCH_TYPE_NPU;
    }
    
    // Enable TAP device creation
    if (strcmp(variable, SAI_KEY_VS_HOSTIF_USE_TAP_DEVICE) == 0) {
        return "true";
    }
    
    return NULL;
}

int profile_get_next_value(
    sai_switch_profile_id_t profile_id,
    const char** variable,
    const char** value)
{
    return -1;  // No more values
}

const sai_service_method_table_t services = {
    .profile_get_value = profile_get_value,
    .profile_get_next_value = profile_get_next_value
};

int main() {
    printf("╔════════════════════════════════════════════╗\n");
    printf("║  SAI Virtual Switch Example Application   ║\n");
    printf("╚════════════════════════════════════════════╝\n\n");
    
    // Setup signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    // Step 1: Initialize SAI
    printf("→ Initializing SAI...\n");
    sai_status_t status = sai_api_initialize(0, &services);
    if (status != SAI_STATUS_SUCCESS) {
        printf("✗ FAILED: sai_api_initialize returned %d\n", status);
        return 1;
    }
    printf("✓ SUCCESS: SAI initialized\n\n");
    
    // Step 2: Get switch API
    printf("→ Getting switch API...\n");
    sai_switch_api_t *switch_api = NULL;
    status = sai_api_query(SAI_API_SWITCH, (void**)&switch_api);
    if (status != SAI_STATUS_SUCCESS) {
        printf("✗ FAILED: sai_api_query returned %d\n", status);
        return 1;
    }
    printf("✓ SUCCESS: Got switch API\n\n");
    
    // Step 3: Create switch
    printf("→ Creating virtual switch...\n");
    sai_object_id_t switch_id;
    sai_attribute_t attr;
    
    attr.id = SAI_SWITCH_ATTR_INIT_SWITCH;
    attr.value.booldata = true;
    
    status = switch_api->create_switch(&switch_id, 1, &attr);
    if (status != SAI_STATUS_SUCCESS) {
        printf("✗ FAILED: create_switch returned %d\n", status);
        return 1;
    }
    printf("✓ SUCCESS: Created switch with ID: 0x%016lx\n\n", switch_id);
    
    // Step 4: Get switch attributes
    printf("→ Querying switch information...\n");
    
    // Get number of ports
    attr.id = SAI_SWITCH_ATTR_PORT_NUMBER;
    status = switch_api->get_switch_attribute(switch_id, 1, &attr);
    if (status == SAI_STATUS_SUCCESS) {
        printf("  • Number of ports: %u\n", attr.value.u32);
    }
    
    // Get port list
    sai_object_id_t port_list[256];
    attr.id = SAI_SWITCH_ATTR_PORT_LIST;
    attr.value.objlist.count = 256;
    attr.value.objlist.list = port_list;
    
    status = switch_api->get_switch_attribute(switch_id, 1, &attr);
    uint32_t port_count = 0;
    if (status == SAI_STATUS_SUCCESS) {
        port_count = attr.value.objlist.count;
        printf("  • Port list (%u ports):\n", port_count);
        for (uint32_t i = 0; i < port_count && i < 5; i++) {
            printf("    - Port %u: 0x%016lx\n", i, port_list[i]);
        }
        if (port_count > 5) {
            printf("    - ... and %u more\n", port_count - 5);
        }
    }
    printf("\n");
    
    // Step 5: Get port API and configure ports
    printf("→ Configuring ports...\n");
    sai_port_api_t *port_api = NULL;
    status = sai_api_query(SAI_API_PORT, (void**)&port_api);
    
    if (status == SAI_STATUS_SUCCESS && port_count > 0) {
        // Bring up first port
        sai_object_id_t first_port = port_list[0];
        
        attr.id = SAI_PORT_ATTR_ADMIN_STATE;
        attr.value.booldata = true;
        status = port_api->set_port_attribute(first_port, &attr);
        
        if (status == SAI_STATUS_SUCCESS) {
            printf("  • Set port 0x%016lx admin state to UP\n", first_port);
        }
        
        // Check operational status
        attr.id = SAI_PORT_ATTR_OPER_STATUS;
        status = port_api->get_port_attribute(first_port, 1, &attr);
        if (status == SAI_STATUS_SUCCESS) {
            const char *oper_status[] = {"UNKNOWN", "UP", "DOWN", "TESTING"};
            printf("  • Port 0x%016lx oper status: %s\n", 
                   first_port, 
                   oper_status[attr.value.s32]);
        }
    }
    printf("\n");
    
    // Step 6: Display runtime information
    printf("╔════════════════════════════════════════════╗\n");
    printf("║       Virtual Switch is RUNNING!          ║\n");
    printf("╚════════════════════════════════════════════╝\n\n");
    
    printf("You can now:\n");
    printf("  • Check TAP devices: ip link show | grep tap\n");
    printf("  • View logs: Check console output\n");
    printf("  • Send packets: Use ping, iperf, etc.\n");
    printf("\nPress Ctrl+C to stop...\n\n");
    
    // Keep running until signal received
    while (keep_running) {
        sleep(1);
    }
    
    // Cleanup
    printf("\n→ Cleaning up...\n");
    sai_api_uninitialize();
    printf("✓ SAI uninitialized\n");
    printf("\nGoodbye!\n");
    
    return 0;
}

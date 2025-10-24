#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sai/sai.h>

/*
 * SAI Application Example with Broadcom Implementation
 * 
 * This demonstrates initializing SAI with the Broadcom SDK
 * and querying basic switch information.
 */

// Service method table callbacks
const char* profile_get_value(
    sai_switch_profile_id_t profile_id,
    const char* variable)
{
    printf("[Profile] Requested: %s\n", variable);
    
    // Return configuration values as needed
    // Broadcom SAI will query for specific keys
    return NULL;
}

int profile_get_next_value(
    sai_switch_profile_id_t profile_id,
    const char** variable,
    const char** value)
{
    // No more values
    return -1;
}

const sai_service_method_table_t service_table = {
    .profile_get_value = profile_get_value,
    .profile_get_next_value = profile_get_next_value
};

int main() {
    sai_status_t status;
    
    printf("╔════════════════════════════════════════════╗\n");
    printf("║   SAI Broadcom Application Example        ║\n");
    printf("╚════════════════════════════════════════════╝\n\n");
    
    // Step 1: Initialize SAI
    printf("→ Initializing SAI with Broadcom implementation...\n");
    status = sai_api_initialize(0, &service_table);
    if (status != SAI_STATUS_SUCCESS) {
        printf("✗ Failed to initialize SAI: %d\n", status);
        return 1;
    }
    printf("✓ SAI initialized successfully\n\n");
    
    // Step 2: Query Switch API
    printf("→ Querying Switch API...\n");
    sai_switch_api_t *switch_api = NULL;
    status = sai_api_query(SAI_API_SWITCH, (void**)&switch_api);
    if (status != SAI_STATUS_SUCCESS) {
        printf("✗ Failed to query switch API: %d\n", status);
        sai_api_uninitialize();
        return 1;
    }
    printf("✓ Switch API retrieved successfully\n\n");
    
    // Step 3: Display API information
    printf("SAI API Information:\n");
    printf("  • Switch API pointer: %p\n", (void*)switch_api);
    printf("  • API has %lu function pointers\n", sizeof(*switch_api) / sizeof(void*));
    printf("\n");
    
    printf("Available Switch API Functions:\n");
    printf("  • create_switch:        %p\n", (void*)switch_api->create_switch);
    printf("  • remove_switch:        %p\n", (void*)switch_api->remove_switch);
    printf("  • set_switch_attribute: %p\n", (void*)switch_api->set_switch_attribute);
    printf("  • get_switch_attribute: %p\n", (void*)switch_api->get_switch_attribute);
    printf("\n");
    
    // Step 4: Query other APIs
    printf("→ Querying other SAI APIs...\n");
    
    sai_port_api_t *port_api = NULL;
    if (sai_api_query(SAI_API_PORT, (void**)&port_api) == SAI_STATUS_SUCCESS) {
        printf("✓ Port API available\n");
    }
    
    sai_vlan_api_t *vlan_api = NULL;
    if (sai_api_query(SAI_API_VLAN, (void**)&vlan_api) == SAI_STATUS_SUCCESS) {
        printf("✓ VLAN API available\n");
    }
    
    sai_route_api_t *route_api = NULL;
    if (sai_api_query(SAI_API_ROUTE, (void**)&route_api) == SAI_STATUS_SUCCESS) {
        printf("✓ Route API available\n");
    }
    
    sai_fdb_api_t *fdb_api = NULL;
    if (sai_api_query(SAI_API_FDB, (void**)&fdb_api) == SAI_STATUS_SUCCESS) {
        printf("✓ FDB API available\n");
    }
    printf("\n");
    
    // Step 5: Cleanup
    printf("→ Cleaning up...\n");
    sai_api_uninitialize();
    printf("✓ SAI uninitialized\n\n");
    
    printf("═══════════════════════════════════════════════\n");
    printf("✓ Successfully connected to Broadcom SAI!\n");
    printf("═══════════════════════════════════════════════\n\n");
    
    printf("Next Steps:\n");
    printf("  1. Call create_switch() to initialize hardware\n");
    printf("  2. Query switch attributes (ports, capabilities)\n");
    printf("  3. Configure VLANs, routes, ACLs, etc.\n");
    printf("  4. Start forwarding traffic!\n\n");
    
    return 0;
}

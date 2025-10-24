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
    
    // Initialize SAI
    printf("Initializing SAI...\n");
    status = sai_api_initialize(0, &service_table);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to initialize SAI: %ld\n", status);
        return 1;
    }
    
    // Query Switch API
    sai_switch_api_t *switch_api = NULL;
    status = sai_api_query(SAI_API_SWITCH, (void**)&switch_api);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to query switch API: %ld\n", status);
        sai_api_uninitialize();
        return 1;
    }
    printf("Switch API retrieved\n");
    
    // Create switch
    printf("Creating switch...\n");
    sai_object_id_t switch_id;
    sai_attribute_t attr;
    attr.id = SAI_SWITCH_ATTR_INIT_SWITCH;
    attr.value.booldata = true;
    
    status = switch_api->create_switch(&switch_id, 1, &attr);
    if (status == SAI_STATUS_SUCCESS) {
        printf("Switch created: 0x%016lx\n", switch_id);
        
        // Query port count
        sai_attribute_t get_attr;
        get_attr.id = SAI_SWITCH_ATTR_PORT_NUMBER;
        status = switch_api->get_switch_attribute(switch_id, 1, &get_attr);
        if (status == SAI_STATUS_SUCCESS) {
            printf("Number of ports: %u\n", get_attr.value.u32);
        }
    } else {
        printf("Switch creation failed: %ld (expected without hardware)\n", status);
    }
    
    // Cleanup
    printf("Cleaning up...\n");
    sai_api_uninitialize();
    printf("Done\n");
    
    return 0;
}

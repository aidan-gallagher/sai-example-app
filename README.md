# SAI Virtual Switch Example

A Docker-based development environment for building and running SAI (Switch Abstraction Interface) applications using the virtual switch implementation.

## What This Does

This project creates a containerized environment that:
- ✅ Builds the SAI Virtual Switch (`libsaivs`)
- ✅ Compiles a simple SAI example application
- ✅ Creates virtual network ports (TAP devices)
- ✅ Demonstrates Layer 2/3 switching and routing

## Quick Start

### Prerequisites
- Docker
- docker-compose
- Linux host (required for TAP devices)

### Build and Run

```bash
# 1. Build the Docker image (first time only, ~10-15 minutes)
docker-compose build

# 2. Start the container
docker-compose up -d

# 3. Enter the container
docker-compose exec sai-vs bash

# 4. Run the example application
./sai_vs_example
```

### What You'll See

```
╔════════════════════════════════════════════╗
║  SAI Virtual Switch Example Application   ║
╚════════════════════════════════════════════╝

→ Initializing SAI...
✓ SUCCESS: SAI initialized

→ Getting switch API...
✓ SUCCESS: Got switch API

→ Creating virtual switch...
✓ SUCCESS: Created switch with ID: 0x2100000000

→ Querying switch information...
  • Number of ports: 32
  • Port list (32 ports):
    - Port 0: 0x1000000000001
    - Port 1: 0x1000000000002
    ...

╔════════════════════════════════════════════╗
║       Virtual Switch is RUNNING!          ║
╚════════════════════════════════════════════╝

Press Ctrl+C to stop...
```

## Verify It's Working

### Inside the Container

```bash
# Check TAP devices were created
ip link show | grep tap

# View port details
ip -d link show tap0

# Monitor packets
tcpdump -i tap0 -n
```

### Test Packet Forwarding

In another terminal (while app is running):

```bash
# Enter the container
docker-compose exec sai-vs bash

# Configure TAP devices
ip link set tap0 up
ip addr add 192.168.1.10/24 dev tap0

# Send test packets
ping 192.168.1.1  # Should go through virtual switch
```

## Project Structure

```
orangeNOS/
├── Dockerfile              # Build environment
├── docker-compose.yml      # Container orchestration
├── Makefile               # Build configuration
├── README.md              # This file
└── src/
    └── sai_vs_example.c   # SAI application code
```

## Development Workflow

### Modify Code

```bash
# Edit source
vim src/sai_vs_example.c

# Rebuild (fast, uses cached layers)
docker-compose exec sai-vs make clean
docker-compose exec sai-vs make

# Run
docker-compose exec sai-vs ./sai_vs_example
```

### Debugging

```bash
# Enable verbose output
export SWSS_LOG_LEVEL=DEBUG

# Run with GDB
gdb ./sai_vs_example
```

## Architecture

```
┌─────────────────────────────────────────┐
│  Your Application (sai_vs_example)      │
│  - Calls SAI C API                      │
└────────────┬────────────────────────────┘
             │
             ↓ Links to
┌─────────────────────────────────────────┐
│  libsaivs.so (Virtual Switch)           │
│  - Implements SAI in software           │
│  - Creates TAP devices                  │
│  - L2/L3 switching & routing            │
└────────────┬────────────────────────────┘
             │
             ↓ System calls
┌─────────────────────────────────────────┐
│  Linux Kernel                           │
│  - TAP device driver                    │
│  - Network stack                        │
└─────────────────────────────────────────┘
```

## What the Virtual Switch Does

- **Layer 2 Switching**: MAC learning, VLAN filtering, forwarding
- **Layer 3 Routing**: IP routing between subnets, ARP resolution
- **Port Management**: Admin state, operational status, statistics
- **FDB Management**: Dynamic MAC learning, aging
- **Virtual Ports**: Creates TAP devices for each switch port

## Troubleshooting

### Container won't start
```bash
# Check logs
docker-compose logs sai-vs

# Rebuild from scratch
docker-compose down
docker-compose build --no-cache
```

### TAP devices not appearing
- Ensure container runs with `privileged: true`
- Check that `SAI_KEY_VS_HOSTIF_USE_TAP_DEVICE` is enabled

### Build fails
- Ensure sufficient disk space (image is ~2GB)
- Check internet connection (downloads packages)
- Try: `docker system prune -a` and rebuild

## Next Steps

1. **Add VLANs**: Configure VLAN tagging and filtering
2. **Configure Routing**: Set up IP routing between subnets
3. **Add ACLs**: Implement access control lists
4. **Test Performance**: Benchmark packet forwarding rates
5. **Integration**: Connect to SONiC components (syncd, orchagent)

## Resources

- [SAI Specification](https://github.com/opencomputeproject/SAI)
- [SONiC Documentation](https://github.com/sonic-net/SONiC/wiki)
- [Virtual Switch Code](https://github.com/sonic-net/sonic-sairedis/tree/master/vslib)

## License

This example code is provided as-is for educational purposes.

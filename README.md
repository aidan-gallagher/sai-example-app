# SAI Application with Broadcom SDK

## Build

```bash
docker build -t sai-app .
```

## Run

```bash
docker run -it sai-app
./sai_app
```

## Project Structure

```
├── Dockerfile           # Build environment
├── debs/
│   └── libsaibcm_*.deb # Broadcom SAI library 
└── src/
    ├── sai_app.c       # SAI application
    └── Makefile        # Build config
```


## SAI Broadcom Implementation

Broadcom SAI implementation is closed source.

In `sonic-buildimage/platform/broadcom/sai.mk` you can see it pulls down https://packages.trafficmanager.net/public/sai/sai-broadcom/SAI_12.3.0_GA/12.3.2.2/dnx/libsaibcm_dnx_12.3.2.2_amd64.deb.


## TODO:
Rather than using broadcom SAI implementation, try to get virtual SAI implementation:
* vslib: https://github.com/sonic-net/sonic-sairedis/tree/master/vslib 
* VPP: https://github.com/sonic-net/sonic-platform-vpp/
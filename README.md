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

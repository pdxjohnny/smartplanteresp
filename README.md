# smartplanteresp

## WiFiManager Library

This may or may not be needed due to the how the `/wifisave` path is hit by
requesting an Image to load to get around the Mixed Content policy blocking a
fetch request. This branch enables CORS.

```console
git clone https://github.com/pdxjohnny/WiFiManager \
  -b cors_all ~/Arduino/libraries/WiFiManager
```

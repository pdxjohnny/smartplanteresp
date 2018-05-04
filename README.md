# smartplanteresp

## WiFiManager Library

Mixed content policy only allows passive resources to load over HTTP
requests. This is a way we can circumvent that policy by requesting a
"passive resource", an Image load, which the server running on the
planter will accept and use the parameters to connect to WiFi.
[Mixed content](https://developer.mozilla.org/en-US/docs/Web/Security/Mixed_content)

To do this we had to modify WiFiManager so that it returns an image.
Therefore to setup this project you will have to have the branch we modified
checked out in your Arduino liraries directory.

### Linux

```console
git clone https://github.com/pdxjohnny/WiFiManager \
  -b cors_all ~/Arduino/libraries/WiFiManager
```

### Windows (git bash)

```console
git clone https://github.com/pdxjohnny/WiFiManager \
  -b cors_all ~/Documents/Arduino/libraries/WiFiManager
```

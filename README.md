LV2 Widener Plugin
---

The purpose of this plugin is to make the sound appear to be 'wider' than it actually is. This is achieved by adding random noise to the phase of the signal.

To install type
```
make install
```


## How it works
The steps used in the plugin are to:
 1. take a forward fft on N samples
 2. change the phase of each frequency bin based on how much variation you want
 3. convert the frequency bins back into a signal

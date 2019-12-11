Refer to [Server-Handler](https://github.com/Three-Headed-Lizards/Server-Handler) For project description

# Usage:

Please have opencv4 and libapriltags installed.
Both of these require cmake, make and gnutils. - Refer to their respective installation instructions


```bash
  $ make
```

Other Targets

```bash
  $ make clean # removes symlinks
  $ make format # formats code according to Mozilla format style guides
```



To run the script, run
``` bash
  $ ./run <username>
```

and make sure you provide appropiate camera / webcam permissions (linux users, update your udev)

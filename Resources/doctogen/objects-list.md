There is different kind of objects in Kiwi.

<img alt="Kiwi Objects" src="./img/object_typology.png" width=400px class="centered" />

Here is a list of all available objects :

### Patching

| Object          |                       Description                       | Alias | Shortcut |
|-----------------|---------------------------------------------------------|:-----:|:--------:|
| **newbox**      | Empty new box                                           |       |    `n`   |
| **errorbox**    | Error box (when an object is invalid)                   |       |          |
| **send**        | Send message to matching receive object                 |  *s*  |          |
| **receive**     | Receive messages from a sender bound to the same name   |  *r*  |          |
| **mtof**        | Convert midi notes to frequencies                       |       |          |
| **select**      | Ouput bang if receive matching input                    | *sel* |          |
| **random**      | Ouput random number in a uniform distribution           |       |          |
| **pack**        | Create and outputs a list of messages                   |       |          |
| **unpack**      | Access elements in a list                               |       |          |
| **trigger**     | Trigger events based on received data                   |  *t*  |          |
| **print**       | Print messages into Kiwi console                        |       |          |
| **loadmess**    | Output arguments as message when the patch is loaded    |       |          |
| **delay**       | Delay messages as bang                                  | *del* |          |
| **pipe**        | Delay any message                                       |       |          |
| **metro**       | Output bang at regular time interval                    |       |          |
| **scale**       | Convert a number from one range into another range      |       |          |
| **gate**        | Route message to selected output                        |       |          |
| **switch**      | Output message receive in selected inlet                |       |          |
| **clip**        | Clip a number between a maximum and a minimum value     |       |          |
| **float**       | Store a float value                                     |  *f*  |          |
| **route**       | Route messages according to the first element           |       |          |

### GUI Objects

| Object          |                       Description                       | Alias | Shortcut |
|-----------------|---------------------------------------------------------|:-----:|:--------:|
| **message**     | Send messages or list of messages                       |       |          |
| **slider**      | Graphical slider sending values between 0 and 1         |       |    `s`   |
| **toggle**      | A switch button                                         |       |    `t`   |
| **bang**        | Send a bang message                                     |       |    `b`   |
| **meter~**      | A peak amplitude vu meter                               |       |          |
| **comment**     | Add a comment to the patch                              |       |    `c`   |
| **number~**     | Display audio samples values                            |       |          |
| **number**      | Display a number                                        |       |  `i`/`f` |

### Operators

| Object          |                       Description                       |
|-----------------|---------------------------------------------------------|
| **+**           | Add two numbers                                         |
| **\***          | Multiply two numbers                                    |
| **%**           | Compute the modulo of number in desired basis           |
| **pow**         | Compute exponentiation of a number                      |
| **==**          | Equality operator                                       |
| **!=**          | Inequality operator                                     |
| **>=**          | Greater than operator                                   |
| **>**           | Strictly greater than operator                          |
| **<=**          | Lesser than operator                                    |
| **<**           | Strictly lesser than operator                           |
| **/**           | Divide two numbers                                      |
| **-**           | Substract two numbers                                   |

### Signal

| Object          |                       Description                       |
|-----------------|---------------------------------------------------------|
| **line~**       | Generate an audio ramp signal                           |
| **noise~**      | A white noise generator                                 |
| **sah~**        | Samples and holds an input signal                       |
| **snapshot~**   | Ouput sample signal values                              |
| **adc~**        | Receive audio signals from inputs or external devices   |
| **dac~**        | Send audio signals to outputs or external devices       |
| **osc~**        | A cosine oscillator                                     |
| **phasor~**     | Generate a sawtooth signal                              |
| **sig~**        | Convert numbers into audio signals                      |
| **delaysimple~**| Simple variable delay line with optional feedback value |

### Operators (signal)

| Object          |                       Description                       |
|-----------------|---------------------------------------------------------|
| **!=~**         | Signal inequality operator                              |
| **==~**         | Signal equality operator                                |
| **>=~**         | Signal superior operator                                |
| **>~**          | Signal strict superior operator                         |
| **<=~**         | Signal inferior operator                                |
| **<~**          | Signal strict inferior operator                         |
| **/~**          | Divide two signals                                      |
| **-~**          | Substract two signals                                   |
| **\*~**         | Multiply two signals                                    |
| **+~**          | Add two signals                                         |

### Record And play sound files

| Object          |                       Description                       |
|-----------------|---------------------------------------------------------|
| **sf.play~**    | Play an audio file from disk                            |
| **sf.record~**  | Record an audio file on disk                            |

### Collaborate

| Object          |                       Description                       |
|-----------------|---------------------------------------------------------|
| **hub**         | Send message to other users of the same patch           |

### Others

| Object          |                       Description                       |
|-----------------|---------------------------------------------------------|
| **faust~**      | Edit and run FAUST code                                 |
| **OSC.receive** | Receive Open Sound Control messages over UDP            |
| **OSC.send**    | Send Open Sound Control messages over UDP               |

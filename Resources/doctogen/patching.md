In Kiwi you can either join a shared patch using the Document Browser window or open a local patch from your machine. In this section we will show how to open and manipulate a local patch. Instructions on how to collaborate over a patch are given [here](collaborate).

<img title="Risset Bell" src="./img/RissetBell.png" width="800px">

> [Risset's bell modelling](http://www.alainbonardi.net/songes/modelisation_eng.htm), you can found the patch in the examples.

The patcher above is taken from the Kiwi examples. As you can see, a patch is a set of objects each one carrying out an operation. Control or signal data is sent between objects using links. Grey connections are used to send messages (numbers, strings, lists, bang ect...) whereas dark connections send signals, always from the top to the bottom.

To create a new patcher, press `cmd`/`ctrl` + `n` or go to `File/New Patcher`.

A patch can either be in *edit* or in *play* mode. To switch between *edit* and *play* mode, click on the lock button
<img title="lock" src="./img/locked.png" height=22px/> or press `cmd` + `e` on Mac (`ctrl` + `e` for linux or windows users).

### edit mode

| edit mode |
|:---:|
| <img alt="patcher edit" src="./img/kiwi_edit_video.gif"/> |

In *edit* mode objects and links can be added, removed or edited into the patch.

You can add objects in a patch by first creating an empty box with `n` or by double clicking and type an object name in the text editor.

<img alt="Object box popup" src="./img/object_box_popup.png" class="centered" width=500px />

> Please see the list of [availaible objects](objects-list).

> You can also use availables [shortcuts](shortcuts) to instantiate GUI objects more easily.

### play mode

| play mode |
|:---:|
| <img alt="patcher playing" src="./img/kiwi_play_video_slider.gif"/> |

In *play* mode graphical objects (the *slider* object for example) can be manipulated directly to change the state of the patch that is executed.

The audio processing is by default disabled, to put the audio on, click on the speaker icon on the top left part of the patch <img title="lock" src="./img/dsp_off.png" height=22px/>.

Once you have locked the patch and started the audio processing, in the above example, you could alter the gain of the signal with the *slider* or change the frequency of the oscillator using the *number* box.

### Multiview

<img alt="patcher multiview" src="./img/patcher_multiview.png" width=800px class="centered"/>

> You can open a new view by pressing `cmd`/`ctrl` + `shift`+ `n`.

### Audio Settings

<img title="Audio Settings" src="./img/audio_settings.png" width=500px class="centered"/>

> You can access the audio settings in the `Options/Audio Settings` menu.

In this section we will describe how one can join a patch and collaborate with other users. Once you are logged in, you can access the remote documents from the document browser window.

<img title="first patch" src="./img/document_browser.png" height="500" class="centered">

As you can see the window displays a list of documents accessible online. They can be access by anyone connected to the same server. You can download, delete, upload, rename, duplicate patches from this window by right clicking on an item. Sorting documents using various criteria is also possible.

Double click on a document to join an online session and start collaborating with other users. You can click on the top left icon of the patcher to show the users that are currently editing or playing with the patch.

<img alt="user list popup" src="./img/patcher_connected_user_list_popup.png" width="250">
<img alt="FM Synth collaborate" src="./img/FM_synth_collab.png" width="500">

Objects currently selected by other users are highlighted in <span class="inline-block orange">orange</span>, whereas your own selection is shown in <span class="inline-block blue">blue</span>.

You can now edit and/or play the patch as you will with the others in realtime. Every modification you make in the structure of the patch will automatically be synced between peers and saved on the server.

On the other hand, when you play with the patch, its state is not shared with the others. Indeed the execution of the patch stay local so you can control parameters independently and isolate or customize audio rendering.

| User A | User B |
|:---:|:---:|
| <img alt="User A" src="./img/kiwi_play_video_number.gif"/> | <img alt="User B" src="./img/kiwi_play_video_slider.gif"/> |

If you want to share some data of the patch with other users or synchronize GUI objects, you can do it via a specific object of the language called a **hub** that broadcasts messages to every users connected to the same document through the server.

<img alt="Using the kiwi hub object" src="./img/hub_server_communication.png" width="600" class="centered"/>

> Open the **hub** help patch in the Help menu for more infos

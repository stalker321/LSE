# LSE

**LSE** is a local search engine written in c++ that is capable of processing requests from a JSON file or sent requests over the network and finding suitable answers from available txt or rtf files.

## Necessary *libraries* for the application to work
- **QT** (5 or 6):
- - Core;
- - Network;
- - Concurrent;
- **Windows**:
- - psapi.lib;

## Available commands
| **Commands** | **Description**                 |
|--------------|---------------------------------|
| **?** | displays the available commands|
| **i** | displays information about the resources used|
| **b** | add a file to the database|
| **m** | output of the initial information|
| **t** | verification by request (not displayed in the history)|
| **r** | set the maximum value of the output responses|
| **l** | view/edit the blacklist|
| **e** | exiting the program|

## Info
At the moment, the program does not fully implement work with network requests.

<br>It is better to use **MinGW** as a compilation tool.
<br>When working with **MinGW**, there are fewer problems when using the Russian language.
<br>Use new versions of **MinGW**, because an error may occur in older versions when working with std::filesystem.
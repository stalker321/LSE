# LSE

**LSE** is a local search engine written in c++ that is capable of processing requests from a JSON file or sent requests over the network and finding suitable answers from available txt or rtf files.

## Necessary *libraries* for the application to work
- **QT** (5 or 6):
- - Core;
- - Network;
- - Concurrent;

### If you work in Windows
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
| **l** | view/edit the blacklist|
| **r** | set the maximum value of the output responses|
| **f** | view the error list|
| **e** | exiting the program|

## Info
At the moment, the program does not fully implement work with network requests.

You can specify the path to resources in config.json or via the console.

If errors occurred during the execution of the program, their list can be viewed both through the console (Available commands) and in the file ErrorLog.txt.

<br>It is better to use **MinGW** as a compilation tool.
<br>When working with **MinGW**, there are fewer problems when using the Russian language.
<br>Use new versions of **MinGW**, because an error may occur in older versions when working with std::filesystem.

## Update
- v1.1.2
- - Work to improve OS compatibility
##
- v1.1.1
- - fixed the order in which responses are displayed/saved.
##
- v1.1.0
- - Added error log output via the console;
- - Now the program will continue to work with an empty database;
- - A relevant response is sent to the network request (so far in json format);
- - A number of changes to improve compatibility with other operating systems (more tests are needed);
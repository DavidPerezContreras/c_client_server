# 📐  C Client-Server example

Creates 2 executables. server.exe is a server and outputs whatever the client sends to it. client.exe is a client that sends "Hello world\naaaaaa"... The server stops listening when it reaches \n.

Uses the event "ServerReady" to notify the client that the server is listening... The client waits until it has received the event and to then send the hello world string.
---

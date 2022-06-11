# Sockets
`Socket` is **one endpoint** of a two-way commuication link between two programs running on the network.

- Sockets are the basic building block of network communication.
- Sockets sit underneath all of the common internet protocols that we use. 

## Client Socket Workflow
1. Client socket is created with a `socket()` call
1. And connectedto a remote adress with the `connect()` call
1. Retrieve data with the `recv()` call


# TSK200: A TIS-100 Emulator

This is a C program that implements the TIS-100 instruction set.

## Features

- [X] Fully functioning TSK nodes.
- [X] File parsing for TSK nodes. (Check the sigh-commands folder for more information)
- [X] A stack module
- [X] Multi node output support
- [ ] TSK nodes running on their own threads.
- [ ] Pipes that connect all nodes together

## Building

For release:

```bash
$ make release
```

For debug:

```bash
$ make debug
```

## Usage

```bash
$ ./tsk [--delay-time TIME] [--max-outputs OUTPUT_LIMIT] [--path NODE_PATH]
```

### Arguments
- `--delay-time` = Add a delay before each node's tick. Useful for actually seeing what's happening.
- `--max-outputs` = Output limit for the output node. **Without this the nodes will run forever**
- `--path` = A path where the node's are defined with both topology and commands

## Node Files

### Topology

Defines the topology for the current node.

- `LEFT:nodeName` A node to the left of the current node
- `RIGHT:nodeName` A node to the right of the current node
- `UP:nodeName` A node directly above the current node
- `DOWN:nodeName` A node directly below the current node
- `OUTPUT:boolean` A node that is an output node. The ACC register will be printed to stdout every time ACC is updated.

View [sample.topo](./sample-commands/sample.topo) for more information.

### TSK Node

View [sample.tsk](./sample-commands/sample.tsk) for more information.

### STK Node (Stack)

Create a file with the `.stk` file extension. No other modifications are needed and the text in the `.stk` file will not be parsed.

## License

MIT, but the TIS-100 instruction set is technically proprietary so...

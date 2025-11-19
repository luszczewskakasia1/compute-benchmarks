Purpose
---------
This is a working document shortly describing the test parameters and their
impact on the test execution - together with simple illustrations (stdout
prints). It appears to be handy when benchmark results needs to be interpreted
- precise understanding of the arguments is mandatory to correctly reason about
the results.


High-level
----------

#### Default mode
The test records subsequent commands (as per specified parameters) into
immediate command-lists (connected into a graph), which then are used to
instantiate and execute it.

#### Emulation mode
Instead of recording commands onto an immediate cmdlist there are
regular-cmdlists, which commands are recorded on.


Measurement args
----------------
There are 3 boolean args determining what is being measured: `mRec`, `mInst`,
`mDest` - for recording, instantiation and execution steps respectively. Not
necessarily all combinations make sense but here are some example prints showing
what is happening:

```
        mRec: 1    mInst: 0    m.Dest: 0
        ------------------------
        START mRec
                recordGraph
        STOP mRec
                instantiateExecutableGraphs
                graph was NOT reset
        (...)


        mRec: 0    mInst: 1    m.Dest: 0
        ------------------------
                recordGraph
        START mInst
                instantiateExecutableGraphs
                graph was NOT reset
        STOP at end of loop
        (...)


        mRec: 0    mInst: 0    m.Dest: 1
        ------------------------
                recordGraph
                instantiateExecutableGraphs
        START mDest
                graph reset
        STOP at end of loop
        (...)


        mRec: 1    mInst: 1    m.Dest: 0
        ------------------------
        START mRec
                recordGraph
                instantiateExecutableGraphs
                graph was NOT reset
        STOP at end of loop
        (...)


        mRec: 0    mInst: 1    m.Dest: 1
        ------------------------
                recordGraph
        START mInst
                instantiateExecutableGraphs
                graph reset
        STOP at end of loop
        (...)


        mRec: 1    mInst: 0    m.Dest: 1
        ------------------------
        START mRec
                recordGraph
        STOP mRec
                instantiateExecutableGraphs
        START mDest
                graph reset
        STOP at end of loop
        (...)


        mRec: 1    mInst: 1    m.Dest: 1
        ------------------------
        START mRec
                recordGraph
                instantiateExecutableGraphs
                graph reset
        STOP at end of loop
        (...)
```


Workload commands args
----------------------
Several args determine how many commands will be recorded and on how many
command-lists as well as how the command-lists are interconnected (forks,
joins).

- `nLvls`: nesting level of a graph, e.g. there will be no forks (nor joins) if
  set to 1 as the lowest-level cmdlist (which cannot fork) will be the only one.

- `nForksInLvl`: number of subsequent sub-cmdlists at each level EXCEPT FOR THE
  LAST ONE (by definition the lowest-level cmdlist has no forks). Each such
  sub-cmdlist starts (by fork point) after the previous one is joined.

- `nCmdSetsInLvl`: number of sets of commands issued AT EACH LEVEL. A single set
  is made up of single or multiple of `appendLaunchKernel` and
  `appendMemoryCopy` commands (tuned by respective args). There is also
  a hardcoded trailing `appendMemoryCopy` added so that each sub-cmdlist is
  joined properly.

- `nAppendKern`: number of `appendLaunchKernels` in a single command-set

- `nAppendCopy`: number or `appendMemoryCopy` copy in a single command-set

Note that the arguments are not completely independent. For example, as
mentioned above, the `nLvls` must be >1 in order to let any fork happen,
otherwise the `nForksInLvl` value has no effect.
Similarly, the total amount of commands at each level:
`nAppendKern + nAppendCopy` must be at least equal to the number of requested
forks (`nForksInLvl`), otherwise some forks won't happen.


The above is best illustrated with the following dumps of manually added print
messages for the recording step (parameter names in the code are not equal to
the arguments described above but are easily mapped).


Recording part - example prints
-------------------------------

#### `--nLvls=1 --nForksInLvl=1 --nCmdSetsInLvl=2 --nAppendKern=2 --nAppendCopy=1`

Simplified example. Note that there is no forks because of `--nLvls=1`, thus
`--nForksInLvl=1` has no effect.

```
  nLevels: 1
  numCommandSetsPerLevel: 2
  numForksPerLevel: 1
  numKernelsInSet: 2
  numCopiesInSet: 1
   |
   | LEVEL: 0   waitEvent (set by parent level): (nil)
   | totalCommands (per level) = numCommandSetsPerLevel * (numKernelsInSet + numCopiesInSet)     : 6
   | forkToCommandRatio (per level): 2147483647
   + ==== cmdSet: 0
   |---- AppendKernel (in current cmdSet): 0
   | forks left for level: 0  >?=  cmds left: 6         (true => FORK)
   | forkSpan: 0  =?=  forkToCommandRatio: 2147483647   (true => FORK)
   | [appendKernel]      cmdlist: 0x5c40a1db7138         signalEvent: (nil)      waitEvent: (nil)
   |---- AppendKernel (in current cmdSet): 1
   | forks left for level: 0  >?=  cmds left: 5         (true => FORK)
   | forkSpan: 1  =?=  forkToCommandRatio: 2147483647   (true => FORK)
   | [appendKernel]      cmdlist: 0x5c40a1db7138         signalEvent: (nil)      waitEvent: (nil)
   |---- MemCopy (in current cmdSet): 0
   | forks left for level: 0  >?=  cmds left: 4         (true => FORK)
   | forkSpan: 2  =?=  forkToCommandRatio: 2147483647   (true => FORK)
   | [appendMemoryCopy]          cmdlist: 0x5c40a1db7138         signalEvent: (nil)      waitEvent: (nil)
   + ==== cmdSet: 1
   |---- AppendKernel (in current cmdSet): 0
   | forks left for level: 0  >?=  cmds left: 3         (true => FORK)
   | forkSpan: 3  =?=  forkToCommandRatio: 2147483647   (true => FORK)
   | [appendKernel]      cmdlist: 0x5c40a1db7138         signalEvent: (nil)      waitEvent: (nil)
   |---- AppendKernel (in current cmdSet): 1
   | forks left for level: 0  >?=  cmds left: 2         (true => FORK)
   | forkSpan: 4  =?=  forkToCommandRatio: 2147483647   (true => FORK)
   | [appendKernel]      cmdlist: 0x5c40a1db7138         signalEvent: (nil)      waitEvent: (nil)
   |---- MemCopy (in current cmdSet): 0
   | forks left for level: 0  >?=  cmds left: 1         (true => FORK)
   | forkSpan: 5  =?=  forkToCommandRatio: 2147483647   (true => FORK)
   | [appendMemoryCopy]          cmdlist: 0x5c40a1db7138         signalEvent: (nil)      waitEvent: (nil)

```


#### `--nLvls=2 --nForksInLvl=3 --nCmdSetsInLvl=3 --nAppendKern=1 --nAppendCopy=1`

```
    nLevels: 2
    numCommandSetsPerLevel: 3
    numForksPerLevel: 3
    numKernelsInSet: 1
    numCopiesInSet: 1
     |
     | LEVEL: 0   waitEvent (set by parent level): (nil)
     | totalCommands (per level) = numCommandSetsPerLevel * (numKernelsInSet + numCopiesInSet)     : 6
     | forkToCommandRatio (per level): 2
     + ==== cmdSet: 0
     |---- AppendKernel (in current cmdSet): 0
     | forks left for level: 3  >?=  cmds left: 6         (true => FORK)
     | forkSpan: 0  =?=  forkToCommandRatio: 2    (true => FORK)
     | [appendKernel]      cmdlist: 0x603e69825138         signalEvent: (nil)      waitEvent: (nil)
     |---- MemCopy (in current cmdSet): 0
     | forks left for level: 3  >?=  cmds left: 5         (true => FORK)
     | forkSpan: 1  =?=  forkToCommandRatio: 2    (true => FORK)
     | [appendMemoryCopy]          cmdlist: 0x603e69825138         signalEvent: (nil)      waitEvent: (nil)
     + ==== cmdSet: 1
     |---- AppendKernel (in current cmdSet): 0
     | forks left for level: 3  >?=  cmds left: 4         (true => FORK)
     | forkSpan: 2  =?=  forkToCommandRatio: 2    (true => FORK)
     | [appendKernel]      cmdlist: 0x603e69825138         signalEvent: 0x603e6982da68     waitEvent: (nil)
     | > FORK to cmdList: 0x603e69837518
         |
         | LEVEL: 1       waitEvent (set by parent level): 0x603e6982da68
         | totalCommands (per level) = numCommandSetsPerLevel * (numKernelsInSet + numCopiesInSet)         : 6
         | forkToCommandRatio (per level): 2147483647
         + ==== cmdSet: 0
         |---- AppendKernel (in current cmdSet): 0
         | forks left for level: 0  >?=  cmds left: 6     (true => FORK)
         | forkSpan: 0  =?=  forkToCommandRatio: 2147483647       (true => FORK)
         | [appendKernel]          cmdlist: 0x603e69837518         signalEvent: (nil)      waitEvent: 0x603e6982da68
         |---- MemCopy (in current cmdSet): 0
         | forks left for level: 0  >?=  cmds left: 5     (true => FORK)
         | forkSpan: 1  =?=  forkToCommandRatio: 2147483647       (true => FORK)
         | [appendMemoryCopy]      cmdlist: 0x603e69837518         signalEvent: (nil)      waitEvent: (nil)
         + ==== cmdSet: 1
         |---- AppendKernel (in current cmdSet): 0
         | forks left for level: 0  >?=  cmds left: 4     (true => FORK)
         | forkSpan: 2  =?=  forkToCommandRatio: 2147483647       (true => FORK)
         | [appendKernel]          cmdlist: 0x603e69837518         signalEvent: (nil)      waitEvent: (nil)
         |---- MemCopy (in current cmdSet): 0
         | forks left for level: 0  >?=  cmds left: 3     (true => FORK)
         | forkSpan: 3  =?=  forkToCommandRatio: 2147483647       (true => FORK)
         | [appendMemoryCopy]      cmdlist: 0x603e69837518         signalEvent: (nil)      waitEvent: (nil)
         + ==== cmdSet: 2
         |---- AppendKernel (in current cmdSet): 0
         | forks left for level: 0  >?=  cmds left: 2     (true => FORK)
         | forkSpan: 4  =?=  forkToCommandRatio: 2147483647       (true => FORK)
         | [appendKernel]          cmdlist: 0x603e69837518         signalEvent: (nil)      waitEvent: (nil)
         |---- MemCopy (in current cmdSet): 0
         | forks left for level: 0  >?=  cmds left: 1     (true => FORK)
         | forkSpan: 5  =?=  forkToCommandRatio: 2147483647       (true => FORK)
         | [appendMemoryCopy]      cmdlist: 0x603e69837518         signalEvent: (nil)      waitEvent: (nil)
         |---- MemCopy (TRAILING)
         | [appendMemoryCopy]      cmdlist: 0x603e69837518         signalEvent: 0x603e69837138     waitEvent: (nil)
     | < JOIN from cmdList: 0x603e69837518
     |---- MemCopy (in current cmdSet): 0
     | forks left for level: 2  >?=  cmds left: 3         (true => FORK)
     | forkSpan: 1  =?=  forkToCommandRatio: 2    (true => FORK)
     | [appendMemoryCopy]          cmdlist: 0x603e69825138         signalEvent: (nil)      waitEvent: 0x603e69837138
     + ==== cmdSet: 2
     |---- AppendKernel (in current cmdSet): 0
     | forks left for level: 2  >?=  cmds left: 2         (true => FORK)
     | forkSpan: 2  =?=  forkToCommandRatio: 2    (true => FORK)
     | [appendKernel]      cmdlist: 0x603e69825138         signalEvent: 0x603e6982da68     waitEvent: (nil)
     | > FORK to cmdList: 0x603e69837518
         |
         | LEVEL: 1       waitEvent (set by parent level): 0x603e6982da68
         | totalCommands (per level) = numCommandSetsPerLevel * (numKernelsInSet + numCopiesInSet)         : 6
         | forkToCommandRatio (per level): 2147483647
         + ==== cmdSet: 0
         |---- AppendKernel (in current cmdSet): 0
         | forks left for level: 0  >?=  cmds left: 6     (true => FORK)
         | forkSpan: 0  =?=  forkToCommandRatio: 2147483647       (true => FORK)
         | [appendKernel]          cmdlist: 0x603e69837518         signalEvent: (nil)      waitEvent: 0x603e6982da68
         |---- MemCopy (in current cmdSet): 0
         | forks left for level: 0  >?=  cmds left: 5     (true => FORK)
         | forkSpan: 1  =?=  forkToCommandRatio: 2147483647       (true => FORK)
         | [appendMemoryCopy]      cmdlist: 0x603e69837518         signalEvent: (nil)      waitEvent: (nil)
         + ==== cmdSet: 1
         |---- AppendKernel (in current cmdSet): 0
         | forks left for level: 0  >?=  cmds left: 4     (true => FORK)
         | forkSpan: 2  =?=  forkToCommandRatio: 2147483647       (true => FORK)
         | [appendKernel]          cmdlist: 0x603e69837518         signalEvent: (nil)      waitEvent: (nil)
         |---- MemCopy (in current cmdSet): 0
         | forks left for level: 0  >?=  cmds left: 3     (true => FORK)
         | forkSpan: 3  =?=  forkToCommandRatio: 2147483647       (true => FORK)
         | [appendMemoryCopy]      cmdlist: 0x603e69837518         signalEvent: (nil)      waitEvent: (nil)
         + ==== cmdSet: 2
         |---- AppendKernel (in current cmdSet): 0
         | forks left for level: 0  >?=  cmds left: 2     (true => FORK)
         | forkSpan: 4  =?=  forkToCommandRatio: 2147483647       (true => FORK)
         | [appendKernel]          cmdlist: 0x603e69837518         signalEvent: (nil)      waitEvent: (nil)
         |---- MemCopy (in current cmdSet): 0
         | forks left for level: 0  >?=  cmds left: 1     (true => FORK)
         | forkSpan: 5  =?=  forkToCommandRatio: 2147483647       (true => FORK)
         | [appendMemoryCopy]      cmdlist: 0x603e69837518         signalEvent: (nil)      waitEvent: (nil)
         |---- MemCopy (TRAILING)
         | [appendMemoryCopy]      cmdlist: 0x603e69837518         signalEvent: 0x603e69837138     waitEvent: (nil)
     | < JOIN from cmdList: 0x603e69837518
     |---- MemCopy (in current cmdSet): 0
     | forks left for level: 1  >?=  cmds left: 1         (true => FORK)
     | forkSpan: 1  =?=  forkToCommandRatio: 2    (true => FORK)
     | [appendMemoryCopy]          cmdlist: 0x603e69825138         signalEvent: 0x603e6982da68     waitEvent: 0x603e69837138
     | > FORK to cmdList: 0x603e69837518
         |
         | LEVEL: 1       waitEvent (set by parent level): 0x603e6982da68
         | totalCommands (per level) = numCommandSetsPerLevel * (numKernelsInSet + numCopiesInSet)         : 6
         | forkToCommandRatio (per level): 2147483647
         + ==== cmdSet: 0
         |---- AppendKernel (in current cmdSet): 0
         | forks left for level: 0  >?=  cmds left: 6     (true => FORK)
         | forkSpan: 0  =?=  forkToCommandRatio: 2147483647       (true => FORK)
         | [appendKernel]          cmdlist: 0x603e69837518         signalEvent: (nil)      waitEvent: 0x603e6982da68
         |---- MemCopy (in current cmdSet): 0
         | forks left for level: 0  >?=  cmds left: 5     (true => FORK)
         | forkSpan: 1  =?=  forkToCommandRatio: 2147483647       (true => FORK)
         | [appendMemoryCopy]      cmdlist: 0x603e69837518         signalEvent: (nil)      waitEvent: (nil)
         + ==== cmdSet: 1
         |---- AppendKernel (in current cmdSet): 0
         | forks left for level: 0  >?=  cmds left: 4     (true => FORK)
         | forkSpan: 2  =?=  forkToCommandRatio: 2147483647       (true => FORK)
         | [appendKernel]          cmdlist: 0x603e69837518         signalEvent: (nil)      waitEvent: (nil)
         |---- MemCopy (in current cmdSet): 0
         | forks left for level: 0  >?=  cmds left: 3     (true => FORK)
         | forkSpan: 3  =?=  forkToCommandRatio: 2147483647       (true => FORK)
         | [appendMemoryCopy]      cmdlist: 0x603e69837518         signalEvent: (nil)      waitEvent: (nil)
         + ==== cmdSet: 2
         |---- AppendKernel (in current cmdSet): 0
         | forks left for level: 0  >?=  cmds left: 2     (true => FORK)
         | forkSpan: 4  =?=  forkToCommandRatio: 2147483647       (true => FORK)
         | [appendKernel]          cmdlist: 0x603e69837518         signalEvent: (nil)      waitEvent: (nil)
         |---- MemCopy (in current cmdSet): 0
         | forks left for level: 0  >?=  cmds left: 1     (true => FORK)
         | forkSpan: 5  =?=  forkToCommandRatio: 2147483647       (true => FORK)
         | [appendMemoryCopy]      cmdlist: 0x603e69837518         signalEvent: (nil)      waitEvent: (nil)
         |---- MemCopy (TRAILING)
         | [appendMemoryCopy]      cmdlist: 0x603e69837518         signalEvent: 0x603e69837138     waitEvent: (nil)
     | < JOIN from cmdList: 0x603e69837518
     |---- MemCopy (TRAILING)
     | [appendMemoryCopy]          cmdlist: 0x603e69825138         signalEvent: (nil)      waitEvent: 0x603e69837138

```


#### `--nLvls=4 --nForksInLvl=1 --nCmdSetsInLvl=2 --nAppendKern=1 --nAppendCopy=1`


```
      nLevels: 4
      numCommandSetsPerLevel: 2
      numForksPerLevel: 1
      numKernelsInSet: 1
      numCopiesInSet: 1
       |
       | LEVEL: 0   waitEvent (set by parent level): (nil)
       | totalCommands (per level) = numCommandSetsPerLevel * (numKernelsInSet + numCopiesInSet)     : 4
       | forkToCommandRatio (per level): 4
       + ==== cmdSet: 0
       |---- AppendKernel (in current cmdSet): 0
       | forks left for level: 1  >?=  cmds left: 4         (true => FORK)
       | forkSpan: 0  =?=  forkToCommandRatio: 4    (true => FORK)
       | [appendKernel]      cmdlist: 0x629e465f1138         signalEvent: (nil)      waitEvent: (nil)
       |---- MemCopy (in current cmdSet): 0
       | forks left for level: 1  >?=  cmds left: 3         (true => FORK)
       | forkSpan: 1  =?=  forkToCommandRatio: 4    (true => FORK)
       | [appendMemoryCopy]          cmdlist: 0x629e465f1138         signalEvent: (nil)      waitEvent: (nil)
       + ==== cmdSet: 1
       |---- AppendKernel (in current cmdSet): 0
       | forks left for level: 1  >?=  cmds left: 2         (true => FORK)
       | forkSpan: 2  =?=  forkToCommandRatio: 4    (true => FORK)
       | [appendKernel]      cmdlist: 0x629e465f1138         signalEvent: (nil)      waitEvent: (nil)
       |---- MemCopy (in current cmdSet): 0
       | forks left for level: 1  >?=  cmds left: 1         (true => FORK)
       | forkSpan: 3  =?=  forkToCommandRatio: 4    (true => FORK)
       | [appendMemoryCopy]          cmdlist: 0x629e465f1138         signalEvent: 0x629e465f9a68     waitEvent: (nil)
       | > FORK to cmdList: 0x629e46603518
           |
           | LEVEL: 1       waitEvent (set by parent level): 0x629e465f9a68
           | totalCommands (per level) = numCommandSetsPerLevel * (numKernelsInSet + numCopiesInSet)         : 4
           | forkToCommandRatio (per level): 4
           + ==== cmdSet: 0
           |---- AppendKernel (in current cmdSet): 0
           | forks left for level: 1  >?=  cmds left: 4     (true => FORK)
           | forkSpan: 0  =?=  forkToCommandRatio: 4        (true => FORK)
           | [appendKernel]          cmdlist: 0x629e46603518         signalEvent: (nil)      waitEvent: 0x629e465f9a68
           |---- MemCopy (in current cmdSet): 0
           | forks left for level: 1  >?=  cmds left: 3     (true => FORK)
           | forkSpan: 1  =?=  forkToCommandRatio: 4        (true => FORK)
           | [appendMemoryCopy]      cmdlist: 0x629e46603518         signalEvent: (nil)      waitEvent: (nil)
           + ==== cmdSet: 1
           |---- AppendKernel (in current cmdSet): 0
           | forks left for level: 1  >?=  cmds left: 2     (true => FORK)
           | forkSpan: 2  =?=  forkToCommandRatio: 4        (true => FORK)
           | [appendKernel]          cmdlist: 0x629e46603518         signalEvent: (nil)      waitEvent: (nil)
           |---- MemCopy (in current cmdSet): 0
           | forks left for level: 1  >?=  cmds left: 1     (true => FORK)
           | forkSpan: 3  =?=  forkToCommandRatio: 4        (true => FORK)
           | [appendMemoryCopy]      cmdlist: 0x629e46603518         signalEvent: 0x629e46602d58     waitEvent: (nil)
           | > FORK to cmdList: 0x629e4660ca18
               |
               | LEVEL: 2   waitEvent (set by parent level): 0x629e46602d58
               | totalCommands (per level) = numCommandSetsPerLevel * (numKernelsInSet + numCopiesInSet)     : 4
               | forkToCommandRatio (per level): 4
               + ==== cmdSet: 0
               |---- AppendKernel (in current cmdSet): 0
               | forks left for level: 1  >?=  cmds left: 4         (true => FORK)
               | forkSpan: 0  =?=  forkToCommandRatio: 4    (true => FORK)
               | [appendKernel]      cmdlist: 0x629e4660ca18         signalEvent: (nil)      waitEvent: 0x629e46602d58
               |---- MemCopy (in current cmdSet): 0
               | forks left for level: 1  >?=  cmds left: 3         (true => FORK)
               | forkSpan: 1  =?=  forkToCommandRatio: 4    (true => FORK)
               | [appendMemoryCopy]          cmdlist: 0x629e4660ca18         signalEvent: (nil)      waitEvent: (nil)
               + ==== cmdSet: 1
               |---- AppendKernel (in current cmdSet): 0
               | forks left for level: 1  >?=  cmds left: 2         (true => FORK)
               | forkSpan: 2  =?=  forkToCommandRatio: 4    (true => FORK)
               | [appendKernel]      cmdlist: 0x629e4660ca18         signalEvent: (nil)      waitEvent: (nil)
               |---- MemCopy (in current cmdSet): 0
               | forks left for level: 1  >?=  cmds left: 1         (true => FORK)
               | forkSpan: 3  =?=  forkToCommandRatio: 4    (true => FORK)
               | [appendMemoryCopy]          cmdlist: 0x629e4660ca18         signalEvent: 0x629e4660c258     waitEvent: (nil)
               | > FORK to cmdList: 0x629e46615f18
                   |
                   | LEVEL: 3       waitEvent (set by parent level): 0x629e4660c258
                   | totalCommands (per level) = numCommandSetsPerLevel * (numKernelsInSet + numCopiesInSet)         : 4
                   | forkToCommandRatio (per level): 2147483647
                   + ==== cmdSet: 0
                   |---- AppendKernel (in current cmdSet): 0
                   | forks left for level: 0  >?=  cmds left: 4     (true => FORK)
                   | forkSpan: 0  =?=  forkToCommandRatio: 2147483647       (true => FORK)
                   | [appendKernel]          cmdlist: 0x629e46615f18         signalEvent: (nil)      waitEvent: 0x629e4660c258
                   |---- MemCopy (in current cmdSet): 0
                   | forks left for level: 0  >?=  cmds left: 3     (true => FORK)
                   | forkSpan: 1  =?=  forkToCommandRatio: 2147483647       (true => FORK)
                   | [appendMemoryCopy]      cmdlist: 0x629e46615f18         signalEvent: (nil)      waitEvent: (nil)
                   + ==== cmdSet: 1
                   |---- AppendKernel (in current cmdSet): 0
                   | forks left for level: 0  >?=  cmds left: 2     (true => FORK)
                   | forkSpan: 2  =?=  forkToCommandRatio: 2147483647       (true => FORK)
                   | [appendKernel]          cmdlist: 0x629e46615f18         signalEvent: (nil)      waitEvent: (nil)
                   |---- MemCopy (in current cmdSet): 0
                   | forks left for level: 0  >?=  cmds left: 1     (true => FORK)
                   | forkSpan: 3  =?=  forkToCommandRatio: 2147483647       (true => FORK)
                   | [appendMemoryCopy]      cmdlist: 0x629e46615f18         signalEvent: (nil)      waitEvent: (nil)
                   |---- MemCopy (TRAILING)
                   | [appendMemoryCopy]      cmdlist: 0x629e46615f18         signalEvent: 0x629e46615b38     waitEvent: (nil)
               | < JOIN from cmdList: 0x629e46615f18
               |---- MemCopy (TRAILING)
               | [appendMemoryCopy]          cmdlist: 0x629e4660ca18         signalEvent: 0x629e4660c638     waitEvent: 0x629e46615b38
           | < JOIN from cmdList: 0x629e4660ca18
           |---- MemCopy (TRAILING)
           | [appendMemoryCopy]      cmdlist: 0x629e46603518         signalEvent: 0x629e46603138     waitEvent: 0x629e4660c638
       | < JOIN from cmdList: 0x629e46603518
       |---- MemCopy (TRAILING)
       | [appendMemoryCopy]          cmdlist: 0x629e465f1138         signalEvent: (nil)      waitEvent: 0x629e46603138
```


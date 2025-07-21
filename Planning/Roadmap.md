# CorePanel Roadmap
## As of July 19, 2025

**Frontend**
- Usage + Temp graphs for main window
    - Reusable, parent graph class
        - A few hours?
    - Style graphs to be fancy
        - Few hours
    - Pipe information to graphs
        - Waiting on C# .NET issue to be solved
        - After that, few hours
    - Figure out stupid widgets/frames in Qt for easy copy and paste
        - Few hours, Qt is relentless
    - CPU
    - GPU
    - RAM
    - Disk (Optional but cool)

**Backend**
- Read data from hardware
    - Get C# .NET library working with Qt program
        - Having issues with calling that .NET program, not sure why
            - Gotta be a few hours away here
        - Maybe key value pairs or JSON?
            - May be an hour or two trying to write it to JSON and then parsing it in Qt app
    - Pushing that data to the graphs, visualization
        - Does data need to be stored somewhere?
            - Design decision? Will have to get data flowing first
        - C++ graphing (Qt)
            - This may be a few hours

**Issues**
- File structure
- C# .NET Program reading when called by itself but not reading when called from Qt app???
- Qt and it's stupid widgets I am so frustrated with them
- Qt in general

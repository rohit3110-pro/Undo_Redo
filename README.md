<code>
╔══════════════════════════════════════════════════════════════════╗
║    ██╗   ██╗███╗   ██╗██████╗  ██████╗     ████████╗██████╗ ███████╗███████╗║
║    ██║   ██║████╗  ██║██╔══██╗██╔═══██╗    ╚══██╔══╝██╔══██╗██╔════╝██╔════╝║
║    ██║   ██║██╔██╗ ██║██║  ██║██║   ██║       ██║   ██████╔╝█████╗  █████╗  ║
║    ██║   ██║██║╚██╗██║██║  ██║██║   ██║       ██║   ██╔══██╗██╔══╝  ██╔══╝  ║
║    ╚██████╔╝██║ ╚████║██████╔╝╚██████╔╝       ██║   ██║  ██║███████╗███████╗║
║     ╚═════╝ ╚═╝  ╚═══╝╚═════╝  ╚═════╝        ╚═╝   ╚═╝  ╚═╝╚══════╝╚══════╝║
║                                                                    ║
║              ███████╗██████╗ ██╗████████╗ ██████╗ ██████╗          ║
║              ██╔════╝██╔══██╗██║╚══██╔══╝██╔═══██╗██╔══██╗         ║
║              █████╗  ██║  ██║██║   ██║   ██║   ██║██████╔╝         ║
║              ██╔══╝  ██║  ██║██║   ██║   ██║   ██║██╔══██╗         ║
║              ███████╗██████╔╝██║   ██║   ╚██████╔╝██║  ██║         ║
║              ╚══════╝╚═════╝ ╚═╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝         ║
╚══════════════════════════════════════════════════════════════════╝
</code>
<hr> Undo redo operations in typical text editors are linear array-like data structures. <code>Alt+z</code> for undos <code>Alt+y</code> for redos <hr> This is what we are used to, but any unintentional typing after an undo just overwrites the data at the front by essentially losing the pointer to the old entries and reassigning it to the current write. This behaviour could at times be unintentional. <hr> This project is a simple attempt to solve this issue using tree data structures. The idea is to store word-level changes after every undo operation into a different tree branch as child nodes, thereby preserving all the previous states in the tree. 

<hr> <h3>To get the project running:</h3>
Clone the repository:
<code>git clone https://github.com/harutoNaiz/undo_redo_tree.git</code>

1. Navigate to the project root and run:
<code>make run</code><br>

2. The editor will pop up in the terminal.
The user manual will guide you.

3. After you are done, run:
<code>make clear</code>
to clear the previous history.

<hr> <h3>Future scope of work includes:</h3> integrating the project into a live text editor by tracking cursor positions.

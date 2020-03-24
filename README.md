# NoteKeeper

Notekeeper is a simple terminal based note keeping application.

## Installation & Setup

- Clone the Repo
- Run "make"
- alias the binary executable to your bashrc file
- run note

## Usage
`note [filename]`

This will open up VIM for writing a note, once closed the file is saved under `~/Documents/NoteKeep/` Each day has its own directory and each file (if not given a name) will be its timestamp.

E.g: 2020-3-23 will be a dir name and can contain the file 11:38:40

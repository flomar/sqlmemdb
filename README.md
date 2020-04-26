# sqlmemdb

Minimalistic Godot engine module exposing an in-memory SQLite database.

This module integrates SQLite directly into Godot so that you can ship a single executable without any additional libraries. The interface doesn't provide much in terms of convenience: You can open an in-memory database, use the query interface to manipulate the database, and get notified through update hooks when changes occur. That's really all there's to it. 

Overall its usefulness is limited, but it fits my requirements perfectly.

There are other and better options for integrating SQLite into Godot, most notably *godot-sqlite* (https://github.com/2shady4u/godot-sqlite), which is much more feature-rich and works as a plugin, so there is no engine rebuild required. This is what you'll most likely want. It's even available in the asset store, so installation is a piece of cake.

Since *sqlmemdb* is MIT-licensed, feel free to do whatever you want with it. Bug fixes are highly appreciated. Feature requests though will likely be denied as this is supposed to be a minimalistic module. If you want extensions, just fork the project and be happy.

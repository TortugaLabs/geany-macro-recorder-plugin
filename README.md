# Macro Recorder plugin for Geany

## About

macrec is a plugin for Geany that provides a simple macro recorder.

This plugin is based on:

- keyrecord - from the [geany-plugins][gp] collection.
- geanymacro - also from the [geany-plugins][gp] collection.
- ctrl-tab - from [geany-ctrl-tab][gct] plugin.

## Building and Installing

Download the plugin from [github][repo]

Then run the following commands:

```bash
$ make
$ sudo make install
```

## Using the Plugin

After having enabled the plugin inside Geany through Geany's plugin manager,
you'll need to setup a keybinding for triggering the Recording and Playback
functions.

Using the plugin is simple. Press Record key and it will start recording.
Press the Record key again it will stop recording.

Press the Playback key and will play the macro.

## License

This plugin is distributed under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 2 of the
License, or (at your option) any later version. You should have received a copy
of the GNU General Public License along with this plugin.  If not, see
<http://www.gnu.org/licenses/>.


To report a bug or ask for a new feature, please use the tracker
on GitHub.

  [gp]: https://github.com/geany/geany-plugins
  [gct]: https://github.com/leifmariposa/geany-ctrl-tab-plugin
  [repo]: https://github.com/TortugaLabs/geany-macro-recorder-plugin



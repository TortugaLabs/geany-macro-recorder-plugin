/*
 *
 *  Copyright (C) 2024 Alejandro Liu <alejandro_liu@hotmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <geanyplugin.h>
#include "utils.h"
#include "Scintilla.h"
//~ #include <glib.h>
//~ #include <glib/gprintf.h>
//~ #include <libgen.h>


#define D(x) /*x*/
//~ #include <glib/gprintf.h>
//~ #define D(x) x


/**********************************************************************/
static const char *PLUGIN_NAME = "macrec";
static const char *PLUGIN_DESCRIPTION = "A simple macro recorder";
static const char *PLUGIN_VERSION = "0.1";
static const char *PLUGIN_AUTHOR = "alejandro_liu@hotmail.com";
static const char *PLUGIN_KEY_NAME = "macrec";

/**********************************************************************/
GeanyPlugin *geany_plugin;
GeanyData *geany_data;


/**********************************************************************/
enum {
  KB_RECORD,
  KB_PLAY,
  KB_COUNT
};

/* structure to hold details of Macro for macro editor */
typedef struct {
  gint message;
  const gchar *description;
} MacroDetailEntry;

/* list of editor messages this plugin can handle & a description */
const MacroDetailEntry MacroDetails[]={
{SCI_CUT,N_("Cut to Clipboard")},
{SCI_COPY,N_("Copy to Clipboard")},
{SCI_PASTE,N_("Paste from Clipboard")},
{SCI_LINECUT,N_("Cut current line to Clipboard")},
{SCI_LINECOPY,N_("Copy current line to Clipboard")},

{SCI_DELETEBACK,N_("Delete character to the left")},
{SCI_CLEAR,N_("Delete character to the right")},
{SCI_DELETEBACKNOTLINE,N_("Delete character to the left (but not newline)")},
{SCI_DELWORDLEFT,N_("Delete up to start of word to the Left")},
{SCI_DELWORDRIGHT,N_("Delete up to start of word to the Right")},
{SCI_DELWORDRIGHTEND,N_("Delete up to end of word to the Right")},
{SCI_DELLINELEFT,N_("Delete to beginning of line")},
{SCI_DELLINERIGHT,N_("Delete to end of line")},
{SCI_LINEDELETE,N_("Delete current line")},
{SCI_BACKTAB,N_("Backwards Tab (deletes tab if nothing after it)")},

{SCI_LINESCROLLDOWN,N_("Scroll Display down a line")},
{SCI_LINESCROLLUP,N_("Scroll Display up a line")},
{SCI_ZOOMIN,N_("Zoom view in")},
{SCI_ZOOMOUT,N_("Zoom view out")},

{SCI_LINEDOWN,N_("Move Cursor Down")},
{SCI_LINEUP,N_("Move Cursor Up")},
{SCI_CHARLEFT,N_("Move Cursor Left")},
{SCI_CHARRIGHT,N_("Move Cursor Right")},
{SCI_WORDLEFT,N_("Move Cursor to start of Word to the Left")},
{SCI_WORDRIGHT,N_("Move Cursor to start of Word to the Right")},
{SCI_WORDPARTLEFT,N_("Move Cursor to start of Part of Word to the Left")},
{SCI_WORDPARTRIGHT,N_("Move Cursor to start of Part of Word to the Right")},
{SCI_HOME,N_("Move Cursor to start of line")},
{SCI_LINEEND,N_("Move Cursor to end of line")},
{SCI_DOCUMENTSTART,N_("Move Cursor to 1st line of Document")},
{SCI_DOCUMENTEND,N_("Move Cursor to last line of document")},
{SCI_PAGEUP,N_("Move Cursor up one Page")},
{SCI_PAGEDOWN,N_("Move Cursor down one Page")},
{SCI_HOMEDISPLAY,N_("Move Cursor to first visible character")},
{SCI_LINEENDDISPLAY,N_("Move Cursor to last visible character")},
{SCI_VCHOME,N_("Move Cursor to 1st non-whitespace character of line, or 1st character of line if\
 already at 1st non-whitespace character")},
{SCI_PARADOWN,N_("Move Cursor to beginning of next paragraph")},
{SCI_PARAUP,N_("Move Cursor up to beginning of current/previous paragraph")},
{SCI_WORDLEFTEND,N_("Move Cursor to end of Word to the Left")},
{SCI_WORDRIGHTEND,N_("Move Cursor to end of Word to the Right")},

{SCI_LINEDOWNEXTEND,N_("Extend Selection down a line")},
{SCI_LINEUPEXTEND,N_("Extend Selection up a line")},
{SCI_CHARLEFTEXTEND,N_("Extend Selection Left a line")},
{SCI_CHARRIGHTEXTEND,N_("Extend Selection Right a line")},
{SCI_WORDLEFTEXTEND,N_("Extend Selection to start of Word to the Left")},
{SCI_WORDRIGHTEXTEND,N_("Extend Selection to start of Word to the Right")},
{SCI_WORDPARTLEFTEXTEND,N_("Extend Selection to start of Part of Word to the Left")},
{SCI_WORDPARTRIGHTEXTEND,N_("Extend Selection to start of Part of Word to the Right")},
{SCI_HOMEEXTEND,N_("Extend Selection to start of line")},
{SCI_LINEENDEXTEND,N_("Extend Selection to end of line")},
{SCI_DOCUMENTSTARTEXTEND,N_("Extend Selection to start of document")},
{SCI_DOCUMENTENDEXTEND,N_("Extend Selection to end of document")},
{SCI_PAGEUPEXTEND,N_("Extend Selection up one Page")},
{SCI_PAGEDOWNEXTEND,N_("Extend Selection down one Page")},
{SCI_HOMEDISPLAYEXTEND,N_("Extend Selection to fist visible character")},
{SCI_LINEENDDISPLAYEXTEND,N_("Extend Selection to last visible character")},
{SCI_VCHOMEEXTEND,N_("Extend Selection to 1st non-whitespace character of line, or 1st character of\
 line if already at 1st non-whitespace character")},
{SCI_PARADOWNEXTEND,N_("Extend Selection to beginning of next paragraph")},
{SCI_PARAUPEXTEND,N_("Extend Selection up to beginning of current/previous paragraph")},
{SCI_WORDLEFTENDEXTEND,N_("Extend Selection to end of Word to the Left")},
{SCI_WORDRIGHTENDEXTEND,N_("Extend Selection to end of Word to the Right")},

{SCI_LINEDOWNRECTEXTEND,N_("Extend Rectangular Selection down a line")},
{SCI_LINEUPRECTEXTEND,N_("Extend Rectangular Selection up a line")},
{SCI_CHARLEFTRECTEXTEND,N_("Extend Rectangular Selection Left a line")},
{SCI_CHARRIGHTRECTEXTEND,N_("Extend Rectangular Selection Right a line")},
{SCI_HOMERECTEXTEND,N_("Extend Rectangular Selection to start of line")},
{SCI_LINEENDRECTEXTEND,N_("Extend Rectangular Selection to end of line")},
{SCI_PAGEUPRECTEXTEND,N_("Extend Rectangular Selection up one Page")},
{SCI_PAGEDOWNRECTEXTEND,N_("Extend Rectangular Selection down one Page")},
{SCI_VCHOMERECTEXTEND,N_("Extend Rectangular Selection to 1st non-whitespace character of line, or\
 1st character of line if already at 1st non-whitespace character")},

{SCI_CANCEL,N_("Cancel Selection")},

{SCI_EDITTOGGLEOVERTYPE,N_("Toggle Insert/Overwrite mode")},
{SCI_TAB,N_("Tab")},
{SCI_NEWLINE,N_("Newline")},

{SCI_REPLACESEL,N_("Insert/replace with \"\"")},

{SCI_LINETRANSPOSE,N_("Swap current line with one above")},
{SCI_LOWERCASE,N_("Change selected text to lowercase")},
{SCI_UPPERCASE,N_("Change selected text to uppercase")},

{SCI_LINEDUPLICATE,N_("Insert duplicate of current line below")},
{SCI_SELECTIONDUPLICATE,N_("Insert duplicate of selected text after selection. If nothing selected,\
 duplicate line")},

{SCI_SEARCHNEXT,N_("Search for next \"\"")},
{SCI_SEARCHPREV,N_("Search for previous \"\"")},
{SCI_SEARCHANCHOR,N_("Set start of search to beginning of selection")},

/* editor commands that don't seem to work well in editing
 * {SCI_FORMFEED,N_("FormFeed")},
 *
 * other commands ommited as they don't appear to do anything different to existing commands
*/
{0,NULL}
};

/* structure to hold details of Macro event */
typedef struct sMacroEvent {
  gint message;
  gulong wparam;
  sptr_t lparam;
  struct sMacroEvent *next;
} MacroEvent;

static MacroEvent *CurrentMacro = NULL;
static MacroEvent *RecordingMacro = NULL;
static MacroEvent NEW_MACRO;

/**********************************************************************/
D(static void log_debug(const gchar* s, ...)
{
	gchar* format = g_strconcat("[MACREC DEBUG] : ", s, "\n", NULL);
	va_list l;
	va_start(l, s);
	g_vprintf(format, l);
	g_free(format);
	va_end(l);
})

/**********************************************************************/

static int lparamIsStr(gint msg) {
  switch (msg) {
  case SCI_REPLACESEL:
  case SCI_SEARCHNEXT:
  case SCI_SEARCHPREV:
    return TRUE;
  }
  return FALSE;
}

static MacroEvent *FreeMacro(MacroEvent *m) {
  MacroEvent *next;
  if (m == &NEW_MACRO) return NULL;
  while (m != NULL) {
    next = m->next;
    if (lparamIsStr(m->message)) g_free((void *)(m->lparam));
    g_free((void *)m);
    m = next;
  }
  return NULL;
}


/**********************************************************************/
static void macrec_start_recording(void) {
  D(log_debug("%s:%s", __FILE__, __FUNCTION__));
  /* start recording process, but quit if error, or user cancels */
  RecordingMacro = &NEW_MACRO;
  /* start actual recording */
  scintilla_send_message(document_get_current()->editor->sci,SCI_STARTRECORD,0,0);
}

static void macrec_stop_recording(void) {
  D(log_debug("%s:%s", __FILE__, __FUNCTION__));
  scintilla_send_message(document_get_current()->editor->sci,SCI_STOPRECORD,0,0);
  if (RecordingMacro == &NEW_MACRO) {
    ui_set_statusbar(TRUE, "Macro recording cancelled...");
    RecordingMacro = NULL;
    return;
  }
  ui_set_statusbar(TRUE, "Macro recorded. Press PLAY key to replay macro...");
  RecordingMacro = NULL;
}

/**********************************************************************/
static void macrec_record(G_GNUC_UNUSED guint key_id) {
  D(log_debug("%s:%s", __FILE__, __FUNCTION__));

  /* can't record if in an empty editor */
  if (document_get_current() == NULL) {
    ui_set_statusbar(TRUE, "No document present");
    D(log_debug("No document present\n"));
    return;
  }
  if (RecordingMacro==NULL) {
    ui_set_statusbar(TRUE, "Macro recording started. Press RECORD key again to stop...");
    macrec_start_recording();
  } else {
    macrec_stop_recording();
  }

}

static void macrec_play(G_GNUC_UNUSED guint key_id) {
  D(log_debug("%s:%s", __FILE__, __FUNCTION__));
  if (document_get_current() == NULL) {
    ui_set_statusbar(TRUE, "No document present");
    D(log_debug("No document present\n"));
    return;
  }
  //~ create_modeless_dialog("Hello world");
  if (CurrentMacro == NULL) {
    D(log_debug("no macro to re-play %s:%s", __FILE__, __FUNCTION__));
    ui_set_statusbar(TRUE, "No macro to replay");
    return;
  }
  if (RecordingMacro != NULL) {
    macrec_stop_recording();
    return;
  }
  ui_set_statusbar(TRUE, "Replaying macro...");

  MacroEvent *me = CurrentMacro;
  ScintillaObject* sci=document_get_current()->editor->sci;
  gchar *clipboardcontents;
  gboolean bFoundAnchor=FALSE;

  scintilla_send_message(sci,SCI_BEGINUNDOACTION,0,0);
  while (me != NULL) {
    if (me->message == SCI_SEARCHANCHOR) bFoundAnchor = TRUE;
    /* possibility that user edited macros might not have anchor before search */
    if((me->message==SCI_SEARCHNEXT || me->message==SCI_SEARCHPREV) && bFoundAnchor==FALSE) {
      scintilla_send_message(sci,SCI_SEARCHANCHOR,0,0);
      bFoundAnchor=TRUE;
    }

    /* search might use clipboard to look for: check & handle */
    if((me->message==SCI_SEARCHNEXT || me->message==SCI_SEARCHPREV) && ((gchar*)me->lparam)==NULL) {
      clipboardcontents=gtk_clipboard_wait_for_text(gtk_clipboard_get(GDK_SELECTION_CLIPBOARD));
      /* ensure there is something in the clipboard */
      if(clipboardcontents==NULL) {
	dialogs_show_msgbox(GTK_MESSAGE_INFO,_("No text in clipboard!"));
	break;
      }
      scintilla_send_message(sci,me->message,me->wparam,(sptr_t)clipboardcontents);
    }
    else
      scintilla_send_message(sci,me->message,me->wparam,me->lparam);

    me = me->next;
  }
  scintilla_send_message(sci,SCI_ENDUNDOACTION,0,0);
  ui_set_statusbar(FALSE, "Macro playback done.");
}

/**********************************************************************/
static gboolean macrec_init(GeanyPlugin *plugin, G_GNUC_UNUSED gpointer pdata) {
  GeanyKeyGroup *key_group;

  D(log_debug("%s:%s", __FILE__, __FUNCTION__));

  key_group = plugin_set_key_group(plugin, PLUGIN_KEY_NAME, KB_COUNT, NULL);
  keybindings_set_item(key_group, KB_RECORD, macrec_record, 0, 0,
			"record", "Start/Stop recording macro", NULL);
  keybindings_set_item(key_group, KB_PLAY, macrec_play, 0, 0,
			"playback", "Playback macro", NULL);

  //~ plugin_signal_connect(geany_plugin, NULL, "document-activate", TRUE, G_CALLBACK(on_document_activate), NULL);
  //~ plugin_signal_connect(geany_plugin, NULL, "document-reload", TRUE, G_CALLBACK(on_document_activate), NULL);
  //~ plugin_signal_connect(geany_plugin, NULL, "document-open", TRUE, G_CALLBACK(on_document_activate), NULL);
  msgwin_status_add("Macro Recorder loaded...");
  return TRUE;
}

/**********************************************************************/
static void macrec_cleanup(G_GNUC_UNUSED GeanyPlugin *plugin, G_GNUC_UNUSED gpointer pdata) {
  D(log_debug("%s:%s", __FILE__, __FUNCTION__));
  RecordingMacro = CurrentMacro = FreeMacro(CurrentMacro);
  msgwin_status_add("Macro Recorder unloaded...");
}

/**********************************************************************/

/* Show help */
static void macrec_help (G_GNUC_UNUSED GeanyPlugin *plugin, G_GNUC_UNUSED gpointer pdata) {
  msgwin_status_add("Launching browser to help file");
  utils_open_browser("https://0ink.net");
}

/**********************************************************************/
static gboolean on_editor_notify(G_GNUC_UNUSED GObject *obj,G_GNUC_UNUSED GeanyEditor *ed,SCNotification *nt,G_GNUC_UNUSED gpointer ud) {
  int i;
  if (nt->nmhdr.code != SCN_MACRORECORD) return FALSE;
  if (RecordingMacro == NULL) return FALSE;

  /* check to see if it's a code we're happy to deal with */
  for(i=0;;i++) {
    /* break if code we can deal with */
    if (nt->message==MacroDetails[i].message) break;

    /* got to end of codes without match: unrecognised */
    if(MacroDetails[i].description==NULL) {
      msgwin_status_add("Unrecognized message: %i %i %i",
			  nt->message,(gint)(nt->wParam),(gint)(nt->lParam));
      return FALSE;
    }
  }
  MacroEvent *me = g_new0(MacroEvent,1);
  me->message = nt->message;
  me->wparam = nt->wParam;
  me->lparam = lparamIsStr(nt->message) ?
		((sptr_t)g_strdup((gchar *)(nt->lParam))) :
		nt->lParam;
  me->next = NULL;
  if (RecordingMacro == &NEW_MACRO) {
    CurrentMacro = RecordingMacro = me;
  } else {
    RecordingMacro->next = me;
    RecordingMacro = me;
  }

  D(log_debug("%s %i %i %i\n", MacroDetails[i].description,
	      nt->message,nt->wParam, nt->lParam));

  D(log_debug("%s:%s", __FILE__, __FUNCTION__));
  return FALSE;
}
static void on_document_close(G_GNUC_UNUSED GObject *obj,G_GNUC_UNUSED  GeanyDocument *doc, G_GNUC_UNUSED gpointer user_data)
{
  D(log_debug("%s:%s", __FILE__, __FUNCTION__));
  if (RecordingMacro != NULL) {
    RecordingMacro = NULL;
  }
}


static PluginCallback macrec_callbacks[] =
{
        { "editor-notify", (GCallback) &on_editor_notify, FALSE, NULL },
        { "document-close", (GCallback) &on_document_close, FALSE, NULL },
        { NULL, NULL, FALSE, NULL }
};



G_MODULE_EXPORT
void geany_load_module(GeanyPlugin *plugin) {
  D(log_debug("%s:%s", __FILE__, __FUNCTION__));

  geany_plugin = plugin;
  plugin->info->name = PLUGIN_NAME;
  plugin->info->description = PLUGIN_DESCRIPTION;
  plugin->info->version = PLUGIN_VERSION;
  plugin->info->author = PLUGIN_AUTHOR;

  plugin->funcs->init = macrec_init;
  plugin->funcs->cleanup = macrec_cleanup;
  plugin->funcs->help = macrec_help;
  plugin->funcs->callbacks = macrec_callbacks;

  GEANY_PLUGIN_REGISTER(plugin, 225);
}


/*
This example application show how to use libnotify

To compile:
gcc `pkg-config --cflags gtk4 libnotify` reminder.c `pkg-config --libs gtk4 libnotify`

*/

#include <gtk/gtk.h>
#include <libnotify/notification.h>
#include <libnotify/notify.h>
#include <string.h>

#define MAX_REMINDERS 10

typedef struct {
  time_t reminder_time; // UNIX timestamp
  char message[256];    // Reminder text
} Reminder;

Reminder reminders[MAX_REMINDERS];
int reminder_count = 0;

gboolean
check_reminders (gpointer user_data) {
  time_t now = time (NULL);

  for (int i = 0; i < reminder_count; i++) {
    if (reminders[i].reminder_time <= now) {
      NotifyNotification *notification = notify_notification_new ("Reminder", reminders[i].message, "dialog-information");
      notify_notification_show (notification, NULL);
      for (int j = i; j < reminder_count - 1; j++) {
        reminders[j] = reminders[j + 1];
      }
      reminder_count--;
      i--;
    }
  }
  return TRUE;
}

// Function to add a reminder
void
add_reminder (time_t reminder_time, const char *message) {
  if (reminder_count < MAX_REMINDERS) {
    reminders[reminder_count].reminder_time = reminder_time;
    strncpy (reminders[reminder_count].message, message, sizeof (reminders[reminder_count].message) - 1);
    reminder_count++;
  }
}

static void
app_activate (GApplication *app, gpointer *user_data) {
  GtkWidget *window;
  window = gtk_application_window_new (GTK_APPLICATION (app));
  gtk_window_set_title (GTK_WINDOW (window), "pr4");
  gtk_window_set_default_size (GTK_WINDOW (window), 400, 300);
  gtk_window_present (GTK_WINDOW (window));
}

int
main (int argc, char **argv) {
  gboolean tmp;
  int stat;
  GtkApplication *app;

  add_reminder (time (NULL) + 10, "Take a break!");

  tmp = notify_init ("app");
  if (!tmp) {
    g_print ("unable to init notification aoo");
    return 1;
  }

  g_timeout_add_seconds (1, check_reminders, NULL);
  app = gtk_application_new ("com.github.shaninalex.cnote.pr1", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (app_activate), NULL);
  stat = g_application_run (G_APPLICATION (app), argc, argv);

  notify_uninit ();
  return 0;
}

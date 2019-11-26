#include <gtk/gtk.h>
#include <string.h>

GtkWidget *entry_0_exc_time;
GtkWidget *entry_1_exc_time;
GtkWidget *entry_2_exc_time;
GtkWidget *entry_3_exc_time;
GtkWidget *entry_4_exc_time;
GtkWidget *entry_5_exc_time;
GtkWidget *entry_0_period;
GtkWidget *entry_1_period;
GtkWidget *entry_2_period;
GtkWidget *entry_3_period;
GtkWidget *entry_4_period;
GtkWidget *entry_5_period;
GtkWidget *label_task_0;
GtkWidget *label_task_1;
GtkWidget *label_task_2;
GtkWidget *label_task_3;
GtkWidget *label_task_4;
GtkWidget *label_task_5;
GtkWidget *label_period;
GtkWidget *label_exec_time;
GtkWidget *label_note;

const gchar *entry_0_text_exec_time;
const gchar *entry_1_text_exec_time;
const gchar *entry_2_text_exec_time;
const gchar *entry_3_text_exec_time;
const gchar *entry_4_text_exec_time;
const gchar *entry_5_text_exec_time;
const gchar *entry_0_text_period;
const gchar *entry_1_text_period;
const gchar *entry_2_text_period;
const gchar *entry_3_text_period;
const gchar *entry_4_text_period;
const gchar *entry_5_text_period;

static void run_scheduler (void)
{
  int entry_0_exec_time_int;

  // Get the execution time
  entry_0_text_exec_time = gtk_entry_get_text (GTK_ENTRY (entry_0_exc_time));
  entry_1_text_exec_time = gtk_entry_get_text (GTK_ENTRY (entry_1_exc_time));
  entry_2_text_exec_time = gtk_entry_get_text (GTK_ENTRY (entry_2_exc_time));
  entry_3_text_exec_time = gtk_entry_get_text (GTK_ENTRY (entry_3_exc_time));
  entry_4_text_exec_time = gtk_entry_get_text (GTK_ENTRY (entry_4_exc_time));
  entry_5_text_exec_time = gtk_entry_get_text (GTK_ENTRY (entry_5_exc_time));

  entry_0_exec_time_int = atoi(entry_0_text_exec_time);
  g_print("NUUUUUM %d", entry_0_exec_time_int);

  // Get the period
  entry_0_text_period = gtk_entry_get_text (GTK_ENTRY (entry_0_period));
  entry_1_text_period = gtk_entry_get_text (GTK_ENTRY (entry_1_period));
  entry_2_text_period = gtk_entry_get_text (GTK_ENTRY (entry_2_period));
  entry_3_text_period = gtk_entry_get_text (GTK_ENTRY (entry_3_period));
  entry_4_text_period = gtk_entry_get_text (GTK_ENTRY (entry_4_period));
  entry_5_text_period = gtk_entry_get_text (GTK_ENTRY (entry_5_period));

  g_print ("entry_0_exc_time contents: %s\n", entry_0_text_exec_time);
  g_print ("entry_1_exc_time contents: %s\n", entry_1_text_exec_time);
  g_print ("entry_2_exc_time contents: %s\n", entry_2_text_exec_time);
  g_print ("entry_3_exc_time contents: %s\n", entry_3_text_exec_time);
  g_print ("entry_4_exc_time contents: %s\n", entry_4_text_exec_time);
  g_print ("entry_5_exc_time contents: %s\n", entry_5_text_exec_time);

  g_print ("entry_0_period contents: %s\n", entry_0_text_period);
  g_print ("entry_1_period contents: %s\n", entry_1_text_period);
  g_print ("entry_2_period contents: %s\n", entry_2_text_period);
  g_print ("entry_3_period contents: %s\n", entry_3_text_period);
  g_print ("entry_4_period contents: %s\n", entry_4_text_period);
  g_print ("entry_5_period contents: %s\n", entry_5_text_period);

}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *button;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Real Time Scheduler");
  gtk_window_set_default_size (GTK_WINDOW (window), 300, 300);
  gtk_container_set_border_width (GTK_CONTAINER (window), 20);

  grid = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET (grid));

  // Create 6 text entries, since it is the maximum of tasks
  label_exec_time = gtk_label_new("Execution time");
  gtk_grid_attach (GTK_GRID (grid), label_exec_time, 1, 0, 1, 1);
  label_period = gtk_label_new("Period");
  gtk_grid_attach (GTK_GRID (grid), label_period, 2, 0, 1, 1);
  label_note = gtk_label_new("Note: leave the fields empty to not use these tasks");
  gtk_grid_attach (GTK_GRID (grid), label_note, 0, 7, 3, 2);

  label_task_0 = gtk_label_new("Task 0");
  gtk_grid_attach (GTK_GRID (grid), label_task_0, 0, 1, 1, 1);
  label_task_1 = gtk_label_new("Task 1");
  gtk_grid_attach (GTK_GRID (grid), label_task_1, 0, 2, 1, 1);
  label_task_2 = gtk_label_new("Task 2");
  gtk_grid_attach (GTK_GRID (grid), label_task_2, 0, 3, 1, 1);
  label_task_3 = gtk_label_new("Task 3");
  gtk_grid_attach (GTK_GRID (grid), label_task_3, 0, 4, 1, 1);
  label_task_4 = gtk_label_new("Task 4");
  gtk_grid_attach (GTK_GRID (grid), label_task_4, 0, 5, 1, 1);
  label_task_5 = gtk_label_new("Task 5");
  gtk_grid_attach (GTK_GRID (grid), label_task_5, 0, 6, 1, 1);

  entry_0_exc_time = gtk_entry_new ();
  gtk_grid_attach (GTK_GRID (grid), entry_0_exc_time, 1, 1, 1, 1);
  entry_1_exc_time = gtk_entry_new ();
  gtk_grid_attach (GTK_GRID (grid), entry_1_exc_time, 1, 2, 1, 1);
  entry_2_exc_time = gtk_entry_new ();
  gtk_grid_attach (GTK_GRID (grid), entry_2_exc_time, 1, 3, 1, 1);
  entry_3_exc_time = gtk_entry_new ();
  gtk_grid_attach (GTK_GRID (grid), entry_3_exc_time, 1, 4, 1, 1);
  entry_4_exc_time = gtk_entry_new ();
  gtk_grid_attach (GTK_GRID (grid), entry_4_exc_time, 1, 5, 1, 1);
  entry_5_exc_time = gtk_entry_new ();
  gtk_grid_attach (GTK_GRID (grid), entry_5_exc_time, 1, 6, 1, 1);

  entry_0_period = gtk_entry_new ();
  gtk_grid_attach (GTK_GRID (grid), entry_0_period, 2, 1, 1, 1);
  entry_1_period = gtk_entry_new ();
  gtk_grid_attach (GTK_GRID (grid), entry_1_period, 2, 2, 1, 1);
  entry_2_period = gtk_entry_new ();
  gtk_grid_attach (GTK_GRID (grid), entry_2_period, 2, 3, 1, 1);
  entry_3_period = gtk_entry_new ();
  gtk_grid_attach (GTK_GRID (grid), entry_3_period, 2, 4, 1, 1);
  entry_4_period = gtk_entry_new ();
  gtk_grid_attach (GTK_GRID (grid), entry_4_period, 2, 5, 1, 1);
  entry_5_period = gtk_entry_new ();
  gtk_grid_attach (GTK_GRID (grid), entry_5_period, 2, 6, 1, 1);

  button = gtk_button_new_with_label ("Schedule");
  gtk_grid_attach (GTK_GRID (grid), button, 0, 9, 3, 1);
  g_signal_connect (button, "clicked", G_CALLBACK (run_scheduler), NULL);

  gtk_widget_show_all (window);
}

int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}

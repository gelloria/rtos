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
GtkWidget *checkbutton_rm;
GtkWidget *checkbutton_edf;
GtkWidget *checkbutton_llf;
GtkWidget *checkbutton_multiple_page;

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

int use_rm = 0;
int use_edf = 0;
int use_llf = 0;
int use_multiple_page = 0;

int entry_0_exec_time_int;
int entry_1_exec_time_int;
int entry_2_exec_time_int;
int entry_3_exec_time_int;
int entry_4_exec_time_int;
int entry_5_exec_time_int;

int entry_0_text_period_int;
int entry_1_text_period_int;
int entry_2_text_period_int;
int entry_3_text_period_int;
int entry_4_text_period_int;
int entry_5_text_period_int;

void get_checkbox_data(){
  use_rm = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (checkbutton_rm));
  use_edf = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (checkbutton_edf));
  use_llf = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (checkbutton_llf));
  use_multiple_page = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (checkbutton_multiple_page));
}

void get_entries_data(){
  // Get the execution time
  entry_0_text_exec_time = gtk_entry_get_text (GTK_ENTRY (entry_0_exc_time));
  entry_1_text_exec_time = gtk_entry_get_text (GTK_ENTRY (entry_1_exc_time));
  entry_2_text_exec_time = gtk_entry_get_text (GTK_ENTRY (entry_2_exc_time));
  entry_3_text_exec_time = gtk_entry_get_text (GTK_ENTRY (entry_3_exc_time));
  entry_4_text_exec_time = gtk_entry_get_text (GTK_ENTRY (entry_4_exc_time));
  entry_5_text_exec_time = gtk_entry_get_text (GTK_ENTRY (entry_5_exc_time));

  // Get the period
  entry_0_text_period = gtk_entry_get_text (GTK_ENTRY (entry_0_period));
  entry_1_text_period = gtk_entry_get_text (GTK_ENTRY (entry_1_period));
  entry_2_text_period = gtk_entry_get_text (GTK_ENTRY (entry_2_period));
  entry_3_text_period = gtk_entry_get_text (GTK_ENTRY (entry_3_period));
  entry_4_text_period = gtk_entry_get_text (GTK_ENTRY (entry_4_period));
  entry_5_text_period = gtk_entry_get_text (GTK_ENTRY (entry_5_period));

  // Cast from string to int
  entry_0_exec_time_int = atoi(entry_0_text_exec_time);
  entry_1_exec_time_int = atoi(entry_1_text_exec_time);
  entry_2_exec_time_int = atoi(entry_2_text_exec_time);
  entry_3_exec_time_int = atoi(entry_3_text_exec_time);
  entry_4_exec_time_int = atoi(entry_4_text_exec_time);
  entry_5_exec_time_int = atoi(entry_5_text_exec_time);

  entry_0_text_period_int = atoi(entry_0_text_period);
  entry_1_text_period_int = atoi(entry_1_text_period);
  entry_2_text_period_int = atoi(entry_2_text_period);
  entry_3_text_period_int = atoi(entry_3_text_period);
  entry_4_text_period_int = atoi(entry_4_text_period);
  entry_5_text_period_int = atoi(entry_5_text_period);

  // Print the results
  g_print ("entry_0_exc_time contents: %d\n", entry_0_exec_time_int);
  g_print ("entry_1_exc_time contents: %d\n", entry_1_exec_time_int);
  g_print ("entry_2_exc_time contents: %d\n", entry_2_exec_time_int);
  g_print ("entry_3_exc_time contents: %d\n", entry_3_exec_time_int);
  g_print ("entry_4_exc_time contents: %d\n", entry_4_exec_time_int);
  g_print ("entry_5_exc_time contents: %d\n", entry_5_exec_time_int);

  g_print ("entry_0_period contents: %d\n", entry_0_text_period_int);
  g_print ("entry_1_period contents: %d\n", entry_1_text_period_int);
  g_print ("entry_2_period contents: %d\n", entry_2_text_period_int);
  g_print ("entry_3_period contents: %d\n", entry_3_text_period_int);
  g_print ("entry_4_period contents: %d\n", entry_4_text_period_int);
  g_print ("entry_5_period contents: %d\n", entry_5_text_period_int);
}

int verify_values(){
  if(entry_0_exec_time_int > entry_0_text_period_int){
    g_print("In the task 0 the execution time is greater than the period. Insert right values.\n");
    return 1;
  }else if(entry_1_exec_time_int > entry_1_text_period_int){
    g_print("In the task 1 the execution time is greater than the period. Insert right values.\n");
    return 1;
  }else if(entry_2_exec_time_int > entry_2_text_period_int){
    g_print("In the task 2 the execution time is greater than the period. Insert right values.\n");
    return 1;
  }else if(entry_3_exec_time_int > entry_3_text_period_int){
    g_print("In the task 3 the execution time is greater than the period. Insert right values.\n");
    return 1;
  }else if(entry_4_exec_time_int > entry_4_text_period_int){
    g_print("In the task 4 the execution time is greater than the period. Insert right values.\n");
    return 1;
  }else if(entry_5_exec_time_int > entry_5_text_period_int){
    g_print("In the task 5 the execution time is greater than the period. Insert right values.\n");
    return 1;
  }

  return 0;
}

static void run_scheduler (void)
{
  get_checkbox_data();
  get_entries_data();
  int values_are_wrong = verify_values();

  // TODO. En lugar de hacer exit, no llamar al scheduler
  if(values_are_wrong) exit(0);
}

static void activate(GtkApplication *app, gpointer user_data)
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
  gtk_grid_attach (GTK_GRID (grid), label_note, 0, 7, 3, 1);

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

  // Create the checkbox to choose the algorithm
  checkbutton_rm = gtk_check_button_new_with_label ("Rate Monothonic");
  gtk_grid_attach (GTK_GRID (grid), checkbutton_rm, 0, 8, 3, 1);
  checkbutton_edf = gtk_check_button_new_with_label ("Earliest Deadline First");
  gtk_grid_attach (GTK_GRID (grid), checkbutton_edf, 0, 9, 3, 1);
  checkbutton_llf = gtk_check_button_new_with_label ("Least Laxity First");
  gtk_grid_attach (GTK_GRID (grid), checkbutton_llf, 0, 10, 3, 1);
  checkbutton_multiple_page = gtk_check_button_new_with_label ("Multiple page");
  gtk_grid_attach (GTK_GRID (grid), checkbutton_multiple_page, 0, 11, 3, 1);

  button = gtk_button_new_with_label ("Schedule");
  gtk_grid_attach (GTK_GRID (grid), button, 0, 12, 3, 1);
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

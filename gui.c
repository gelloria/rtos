#include "mylib.h"

int insert_table_entry(int thread_number,
                       float pi_tmp,
                       char thread_status[],
                       uint percentage){
  GtkTreeIter *iter = NULL;

  // Insert an element in the table
  gtk_list_store_insert_with_values(model, iter, thread_number-1,
                                    COLUMN_THREAD_ID, thread_number,
                                    COLUMN_PI_TMP, pi_tmp,
                                    COLUMN_THREAD_STATUS, thread_status,
                                    COLUMN_PERCENTAGE, percentage,
                                    -1);

  return GUI_OK;
}

int insert_table_clear(){
  gtk_list_store_clear(model);

  return GUI_OK;
}

int gtk_initialize(gint argc, gchar **argv){
  gtk_init(&argc, &argv);

  return GUI_OK;
}

int gtk_model_initialize(){
  // --------------------------------------------------------------------------
  // Create the GUI model
  // --------------------------------------------------------------------------
  model = gtk_list_store_new(COLUMN_MAX,
                             G_TYPE_INT,   /* Thread ID */
                             G_TYPE_FLOAT,    /* Pi tmp */
                             G_TYPE_STRING,    /* Thread status */
                             G_TYPE_UINT    /* Percentage */
                            );

  return GUI_OK;
}

gboolean generate_entries (gpointer thread_data){
  char * thread_status = "";
  uint percentage = 0;
  int workload = 0;
  uint actual_workload = 0;

  // Clear the table before write it
  insert_table_clear();

  for (int i = 1; i <= number_of_threads; i++) {

    // Convert from enum to char the thead status
    if(list_of_threads[i].state == READY){
      thread_status = "Ready";
    }else if(list_of_threads[i].state == WAITING){
      thread_status = "Waiting";
    }else if(list_of_threads[i].state == RUNNING){
      thread_status = "Running";
    }else if(list_of_threads[i].state == FINISHED){
      thread_status = "Finished";
    }else if(list_of_threads[i].state == OPENED){
      thread_status = "Opened";
    }else{
      thread_status = "Unrecognized";
    }

    // Calculate the percentage of work done by the thread
    workload = list_of_threads[i].data.workload;
    actual_workload = list_of_threads[i].data.actual_workload;
    percentage = (uint)(100*actual_workload)/(WORKLOAD_UNIT*workload);

    // Due to round, sometimes the maximum percentage calculated is ~96% but
    // it should be 100%
    if(percentage > 95) percentage = 100;

    insert_table_entry(list_of_threads[i].id,
                       list_of_threads[i].data.result,
                       thread_status,
                       percentage);
  }

  return TRUE;
}

int gtk_view_initialize(void *function_ptr){
  GtkWidget*         view;
  GtkTreeViewColumn* column;
  GtkWidget*         window;

  // --------------------------------------------------------------------------
  // Create the GTK view
  // --------------------------------------------------------------------------
  view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(model));
  g_object_unref(model);

  // Create the thread ID column
  column = gtk_tree_view_column_new_with_attributes("Thread ID",
                                                    gtk_cell_renderer_text_new(),
                                                    "text", COLUMN_THREAD_ID,
                                                    NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

  // Create the thread PI column
  column = gtk_tree_view_column_new_with_attributes("Pi temp",
                                                    gtk_cell_renderer_spin_new(),
                                                    "text", COLUMN_PI_TMP,
                                                    NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

  // Create the thread Thread status column
  column = gtk_tree_view_column_new_with_attributes("Thread status",
                                                    gtk_cell_renderer_text_new(),
                                                    "text", COLUMN_THREAD_STATUS,
                                                    NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

  // Create the thread percentage column
  column = gtk_tree_view_column_new_with_attributes("Percentage",
                                                     gtk_cell_renderer_progress_new(),
                                                     "text", COLUMN_PERCENTAGE,
                                                     "value", COLUMN_PERCENTAGE,
                                                     NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

  // --------------------------------------------------------------------------
  // GTK main
  // --------------------------------------------------------------------------
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_container_add(GTK_CONTAINER(window), view);
  gtk_widget_show_all(window);

  // Create the empty initial threads
  for (size_t i = 1; i <= number_of_threads; i++) {
    insert_table_entry(i, 0, "Ready", 50);
  }

  g_thread_new("", function_ptr, NULL);
  g_timeout_add(20, generate_entries, NULL);
  gtk_main();

  return GUI_OK;
}

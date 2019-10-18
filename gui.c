#include "gui.h"

int insert_table_entry(int thread_number, float pi_tmp, char thread_status[]){
  GtkTreeIter *iter;

  // Insert an element in the table
  gtk_list_store_insert_with_values(model, iter, thread_number-1,
                                    COLUMN_THREAD_ID, thread_number,
                                    COLUMN_PI_TMP, pi_tmp,
                                    COLUMN_THREAD_STATUS, thread_status,
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
                             G_TYPE_STRING    /* Active */
                            );

  return GUI_OK;
}

gboolean generate_entries (gpointer user_data){
  // Create an entry in the table
  /*
  insert_table_entry(1, 3.14, "Running");
  sleep(3);
  insert_table_entry(2, 3.14, "Waiting");
  sleep(3);
  insert_table_entry(3, 3.14, "Waiting");
  sleep(3);
  insert_table_entry(4, 3.14, "Waiting");

  sleep(3);
  insert_table_clear();
  */
  // printf("%s\n", "LALLA 1 ");
  // printf("%s\n", "LALLA 2 ");
  // printf("%s\n", "LALLA 3 ");
  // printf("%s\n", "LALLA 4 ");

  // insert_table_entry(4, 3.14, "Waiting");

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

  // Crear columna de Thread ID
  column = gtk_tree_view_column_new_with_attributes("Thread ID",
                                                    gtk_cell_renderer_text_new(),
                                                    "text", COLUMN_THREAD_ID,
                                                    NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

  // Valor de PI
  column = gtk_tree_view_column_new_with_attributes("Pi temp",
                                                    gtk_cell_renderer_spin_new(),
                                                    "text", COLUMN_PI_TMP,
                                                    NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

  // Activo
  column = gtk_tree_view_column_new_with_attributes("Thread status",
                                                    gtk_cell_renderer_text_new(),
                                                    "text", COLUMN_THREAD_STATUS,
                                                    NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

  // --------------------------------------------------------------------------
  // GTK main
  // --------------------------------------------------------------------------
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_container_add(GTK_CONTAINER(window), view);
  gtk_widget_show_all(window);
  //insert_table_entry(4, 3.14, "Waiting");

  g_thread_new("", function_ptr, (gpointer)window);
  g_timeout_add(16, generate_entries, window);

  printf("\nI'm GTK, message from  main.\n");
  gtk_main();





  return GUI_OK;
}

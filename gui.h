#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include <unistd.h>

#define GUI_OK 0

GtkListStore* model;

// Columns definitions
enum {
    COLUMN_THREAD_ID,
    COLUMN_PI_TMP,
    COLUMN_THREAD_STATUS,
    COLUMN_MAX
};

int insert_table_entry(int thread_number, float pi_tmp, char thread_status[]);

int insert_table_clear();

int gtk_initialize(gint argc, gchar **argv);

int gtk_model_initialize();

gboolean generate_entries (gpointer user_data);

int gtk_view_initialize(void* function_ptr);

#endif

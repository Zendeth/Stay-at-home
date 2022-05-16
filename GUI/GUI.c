#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

GtkWidget		*window;
GtkWidget		*button_next;
GtkWidget		*button_back;
GtkWidget		*button_aboutus;
GtkWidget		*mainpage;
GtkWidget		*qrcodepage;
GtkWidget		*decodepage;
GtkWidget		*encodepage;
GtkBuilder		*builder;

// https://developer-old.gnome.org/gobject/stable/gobject-Signals.html#g-signal-connect

int gui(int argc, char** argv)
{
	gtk_init(&argc, &argv);

	webkit_web_view_get_type();
	webkit_settings_get_type();

	builder = gtk_builder_new_from_file("GUI.glade");

	gtk_builder_connect_signals(builder, NULL);
	
	mainpage = GTK_WIDGET(gtk_builder_get_object(builder, "mainpage"));
	button_aboutus = GTK_WIDGET(gtk_builder_get_object(builder, "button_aboutus"));
	button_start = GTK_WIDGET(gtk_builder_get_object(builder, "button_start"));

	qrcodepage = GTK_WIDGET(gtk_builder_get_object(builder, "qrcodepage"));
	button_back_qr = GTK_WIDGET(gtk_builder_get_object(builder, "button_back_qr"));

	decodepage = GTK_WIDGET(gtk_builder_get_object(builder, "decodepage"));
	button_next_de = GTK_WIDGET(gtk_builder_get_object(builder, "button_next_de"));
	button_back_de = GTK_WIDGET(gtk_builder_get_object(builder, "button_back_de"));
	url = GTK_WIDGET(gtk_builder_get_object(builder, "url"));

	encodepage = GTK_WIDGET(gtk_builder_get_object(builder, "encodepage"));
	button_next_en = GTK_WIDGET(gtk_builder_get_object(builder, "button_next_en"));
	button_back_en = GTK_WIDGET(gtk_builder_get_object(builder, "button_back_en"));
	image = GTK_WIDGET(gtk_builder_get_object(builder, "image"));
	

	//Connects event handlers.
	//g_signal_connect(button_next, "clicked", G_CALLBACK(next_fixed), NULL);
	//g_signal_connect(button_back, "clicked", G_CALLBACK(previous_fixed), NULL);
	g_signal_connect(button_aboutus, "clicked", G_CALLBACK(gtk_widget_hide), &mainpage);
	g_signal_connect(button_start, "clicked", G_CALLBACK(on_button_next_clicked), &mainpage, &qrcodepage);
	g_signal_connect(button_back_qr, "clicked", G_CALLBACK(on_button_back_clicked), &qrcodepage, &mainpage);
	g_signal_connect(button_back_de, "clicked", G_CALLBACK(on_button_back_clicked), &decodepage, &qrcodepage);
	g_signal_connect(button_back_en, "clicked", G_CALLBACK(on_button_back_clicked), &encodepage, &qrcodepage);
	g_signal_connect(button_next_de, "clicked", G_CALLBACK(on_button_next_de_clicked), &qrcodepage, &decodepage);
	g_signal_connect(button_next_en, "clicked", G_CALLBACK(on_button_next_en_clicked), &qrcodepage, &encodepage);

	gtk_widget_show(&mainpage);

	gtk_main();
	
	return EXIT_SUCCESS;

}

void on_button_next_de_clicked(GtkWidget *before, GtkWidget *after, char path[])
{
	gtk_widget_hide(before);
	gtk_widget_show(after);
	url = "";
	// url is the output 
	GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(url));
	// gtk_text_buffer_set_text(buf, url);
	
}

void on_button_next_en_clicked(GtkWidget *before, GtkWidget *after, )
{
	gtk_widget_hide(before);
	gtk_widget_show(after);
	// gtk_image_set_from_file(image, path);
}

void on_button_back_clicked(GtkWidget *before, GtkWidget *after)
{
	gtk_widget_hide(before);
	gtk_widget_show(after);
}
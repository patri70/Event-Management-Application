#include <QApplication>
#include "Service.h"
#include "Repository.h"
#include "GUI.h"
#include "Event.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
	Repository repo("events.txt");
	Service service(repo);

	GUI gui(service);
	gui.setWindowTitle("Event Management System");
	gui.resize(800, 600);
	gui.show();
    
    return app.exec();
}

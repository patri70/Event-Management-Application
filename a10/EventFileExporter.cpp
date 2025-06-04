#include "EventFileExporter.h"
#include <fstream>
#include <Windows.h>
#include <shellapi.h>
#include <iostream>


EventFileExporter::EventFileExporter(const std::string& fileName)
    : filename{ fileName }
{
}

CSVEventExporter::CSVEventExporter(const std::string& filename) : EventFileExporter(filename) {}

void CSVEventExporter::writeToFile() {
    std::ofstream fout{ filename };
	if (!fout.is_open()) {
		std::cerr << "Error: Could not open file " << filename << " for writing.\n";
		return;
	}
	for (const Event& event : events) {
		fout << event.toCSV() << "\n"; 
	}
	fout.close();
}

void CSVEventExporter::displayFile() {
	ShellExecuteA(NULL, "open", filename.c_str(), NULL, NULL, SW_SHOWMAXIMIZED);
}

HTMLEventExporter::HTMLEventExporter(const std::string& filename) : EventFileExporter(filename) {}

void HTMLEventExporter::writeToFile()
{
    std::ofstream fout{ filename };
    if (!fout.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }
    fout << "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"UTF-8\">\n<title>Event List</title>\n</head>\n<body>\n";
    fout << "<table border=\"1\">\n<tr><th>Title</th><th>Description</th><th>Link</th><th>Number of People</th><th>Date</th></tr>\n";
    for (const Event& event : events) {
        fout << event.toHTMLRow() << "\n"; 
    }
    fout << "</table>\n</body>\n</html>";
    fout.close();
}
void HTMLEventExporter::displayFile()
{
	ShellExecuteA(NULL, "open", filename.c_str(), NULL, NULL, SW_SHOWMAXIMIZED);
}
//
// Created by Opala Studios on 21/11/23.
//

#include "FileManager.h"
#include <QFile>
#include <qdebug.h>
#include <rapidcsv.h>
#include "IntervalParser.h"

FileManager::FileManager() :
QObject()
{
}

std::vector<TickData> FileManager::loadCsv(const QString& file) {
    std::vector<TickData> output;

    try {
        rapidcsv::Document doc(file.toStdString(), rapidcsv::LabelParams(1, -1));

        for(int i = 0; i < doc.GetRowCount(); i++)
        {
            TickData data_open;
            TickData data_high;
            TickData data_low;
            TickData data_close;

            //converting ms to sec and add simulated time for the sub tick on the bars
            double timeInMs = doc.GetCell<double>(0,i);
            data_open.time   = timeInMs;
            data_high.time   = timeInMs + 20000;//simulate 20 sec after
            data_low.time    = timeInMs + 40000;//simulate 40 sec after
            data_close.time  = timeInMs + IntervalParser::getTimeIntervalInMilliSeconds(Interval::Interval_1Minute) - 1;//simulate 1 ms before the close time

            data_open.value = doc.GetCell<double>(1,i);
            data_high.value = doc.GetCell<double>(2,i);
            data_low.value = doc.GetCell<double>(3,i);
            data_close.value = doc.GetCell<double>(4,i);

            //0.25 volume for each tick
            double volume = doc.GetCell<double>(5,i)*0.25;
            data_open.volume = volume;
            data_high.volume = volume;
            data_low.volume = volume;
            data_close.volume = volume;

            output.push_back(data_open);
            output.push_back(data_high);
            output.push_back(data_low);
            output.push_back(data_close);
        }

    } catch(std::exception exception) {
        std::cout << "Troubles loading csv file." << "\n";
    }

    return output;
}

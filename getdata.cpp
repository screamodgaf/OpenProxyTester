#include "getdata.h"
#include <vector>
#include <iostream>
#include <QDir>
#include <QCoreApplication>
GetData::GetData( )
{

}

void GetData::nicksGet()
{
    QDir directory  = QCoreApplication::applicationDirPath();
    QString relativePath = directory.absoluteFilePath("nicks.txt");
    //    std::cout << "1. relativePath to nicks.txt " << relativePath.toStdString();
    std::string line = "";
    std::fstream file;
    file.open(relativePath.toStdString(), std::ios::in);
    if(!file.is_open()){
        std::cout << "nicks file is NOT open" << "\n";
        return;
    }
    std::cout << relativePath.toStdString() << " is open\n";
    //    std::fstream File1("B:/Projects/working_proxy_checker-with-ping__SOCKS/nicks.txt", std::ios::in );
    std::string line1;


    while(std::getline(file,line1))
    {
        nicks.push_back(line1);
//        std::cout << line1 << "\n";
    }

    file.close();
    std::cout << "nicks extracted" << "\n";
}

int GetData::ircProxiesGet()
{
    QDir directory  = QCoreApplication::applicationDirPath();
    QString relativePath = directory.absoluteFilePath("list.txt");
    //    std::cout << "2. relativePath to list.txt " << relativePath.toStdString();
    std::string line = "";
    std::fstream file;
    file.open(relativePath.toStdString(), std::ios::in);
    if(!file.is_open()){
        std::cout << " list.txt file is NOT open" << "\n";
        return -1;
    }

    std::string line1;


        while(!file.eof())
        {
            getline(file,line);
//             std::cout <<"line: " <<line<< std::endl;
            size_t coma = line.find(':');
            std::string IP = line.substr(0,coma);
            std::string port = line.substr(coma+1, line.length()-coma);
            bool add = true;
            if(std::find(ircProxies.begin(),ircProxies.end(),IP)!=ircProxies.end())
            {
                auto it = std::find(ircProxies.begin(), ircProxies.end(),IP);
                auto itP = std::find(ircProxiesPort.begin(), ircProxiesPort.end(),port);
                if(it == ircProxies.end() && itP == ircProxiesPort.end())
                {
                    //IP and Port not in vector
                }
                else
                {
                    //IP and Port are in vector but we must check if in the same line
                    auto index = std::distance(ircProxies.begin(), it);
                    auto index2 = std::distance(ircProxiesPort.begin(), itP);
                    if(index==index2) //are present at the same line
                    {
                        add = false;
                    }

                }

            }

            if(add == true)
            {   //add them
                ircProxies.push_back(IP);
                ircProxiesPort.push_back(port);
                ipAmount++;
            }

        }

    file.close();
    std::cout << "ircProxiesSize: " << ircProxies.size() <<std::endl;
    return ircProxies.size();
}



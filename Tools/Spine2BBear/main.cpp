#include <QCoreApplication>
#include "BBUtils.h"

string targetFolder;
string targetAtlasFile;
string targetJsonFile;

spAtlas *pAtlas = nullptr;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    cout << "input target folder, target atlas file, and target json file" << endl;
    // input parameters
    cin >> targetFolder >> targetAtlasFile >> targetJsonFile;

    string atlasFilePath = targetFolder + "/" + targetAtlasFile;
    cout << "load " << atlasFilePath;
    pAtlas = spAtlas_createFromFile(atlasFilePath.c_str(), 0);

    return a.exec();
}

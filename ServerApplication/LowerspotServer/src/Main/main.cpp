
// Project includes ------------------------
#include "Application.h"

// Qt includes -----------------------------
#include <QDir>

//-----------------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  //Set the current directory to the current program path
  QDir::setCurrent(QFileInfo(argv[0]).absolutePath());

  Application application(argc, argv);

  return application.exec();
}

//-----------------------------------------------------------------------------------------------------------------------------

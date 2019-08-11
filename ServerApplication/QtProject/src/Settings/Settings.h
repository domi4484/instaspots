#ifndef MATERIAL_H
#define MATERIAL_H

// Qt includes -----------------------------
#include <QFileInfo>
#include <QString>
#include <QMap>

class Material
{
public:

  class _CONST
  {
  public:
    static const QString LOCATION;
    static const QString FILENAME_EXTENSION;

    class JSON
    {
    public:
      static const QString NAME;

      class VALUES
      {
      public:
          static const QString ROOT_NAME;
          static const QString THICKNESS;
          static const QString SURFACEVALUE;
          static const QString CUTVALUE;
      }; // VALUES
    }; // JSON
  }; // _CONST

  Material(const QFileInfo &qFileInfo);
  void Load();

  QString getName() const;
  QFileInfo getFileInfo() const;

  QList<double> getThicknessList() const;
  double getSurfaceValue(double thickness) const;
  double getCutValue(double thickness) const;

private:

  QFileInfo m_QFileInfo;

  QString m_Name;

  class Value
  {
  public:
    double m_Thickness;
    double m_SurfaceValue;
    double m_CutValue;
  }; // Value

  QMap<double, Value> m_QMap_Values;


};

#endif // MATERIAL_H

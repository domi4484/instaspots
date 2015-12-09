<?php

namespace Instaspots\SpotsBundle\Controller;

class ApplicationHelper
{
  /**
   * Constructor
   */
  public function __construct()
  {

  }

  //-----------------------------------------------------------------------------------------------------------------------------

  /**
   * compareVersions
   *
   * version1 >  version2 -> return 1
   * version1 == version2 -> return 0
   * version1 <  version2 -> return -1
   */
  public static function compareVersions($version1,
                                         $version2) const
  {
    QRegularExpression regexp("V(\\d+).(\\d+).(\\d+)");

    // Match version1
    int first1  = 0;
    int second1 = 0;
    int third1  = 0;
    {
      $version1Array = explode('', substr($version1, 1));

      first1  = (int)$version1Array[0];
      second1 = (int)$version1Array[1];
      third1  = (int)$version1Array[2];
    }

    // Match version2
    int first2  = 0;
    int second2 = 0;
    int third2  = 0;
    {
      $version2Array = explode('', substr($version2, 1));

      first2  = (int)$version2Array[0];
      second2 = (int)$version2Array[1];
      third2  = (int)$version2Array[2];
    }

    if(first1 > first2)
    {
      return 1;
    }
    else if(first1 < first2)
    {
      return -1;
    }
    else
    {
      if(second1 > second2)
      {
        return 1;
      }
      else if(second1 < second2)
      {
        return -1;
      }
      else
      {
        if(third1 > third2)
        {
          return 1;
        }
        else if(third1 < third2)
        {
          return -1;
        }
        else
        {
          return 0;
        }
      }
    }
  }
}

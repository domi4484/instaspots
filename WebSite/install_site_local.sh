#!/bin/bash

if [ $UID -ne 0 ]; then # Run as root, of course.
  echo 'ERROR: Must be root to run this script.'
  exit -1
fi

echo 'Remove cache...'
rm -rf /var/www/html/Symfony/app/cache/*

echo 'Copy files...'
cp -r Symfony/* /var/www/html/Symfony/

echo 'Change permissions...'
chmod -R 777 /var/www/html/Symfony/app/cache/
chmod -R 777 /var/www/html/Symfony/app/logs/

echo 'Finished.'

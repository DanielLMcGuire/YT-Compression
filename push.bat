@echo off

git add .
git commit -m %1

git tag -a %2 -m "Release %2"

git pull
git push origin main
git push origin %2



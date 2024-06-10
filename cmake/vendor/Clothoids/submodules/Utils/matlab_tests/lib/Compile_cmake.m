clear all;
clear functions;
clear mex;
clc;

if ismac
  oldPath = getenv('PATH');
  newPath = strcat(oldPath, pathsep, '/usr/local/bin'); % on MAC
  setenv('PATH', newPath);
elseif isunix
elseif ispc
  oldPath = getenv('PATH');
  newPath = strcat(oldPath, pathsep, 'C:\Program Files\CMake\bin'); % on Windows
  setenv('PATH', newPath);
end

old_dir = cd(fileparts(which(mfilename)));
cd('..');
%system('rmdir /S build');
numcores = feature('numcores');
if isunix
  system('cmake -G "Ninja" -Bbuild .');
  system('ninja -C build');
elseif ispc
  system('cmake -G "Ninja" -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON -Bbuild .');
  system('ninja -C build');
end

cd(old_dir);

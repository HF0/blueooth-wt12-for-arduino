echo "Generating doxygen documentation..."
doxygen doxygenconfig.properties
echo Doxygen finished

moxygen doxygenoutputxml


# sOS - CSC/CPE 159 Project

**Team:** Quinn Roemer, Andrew Larson, Joshua Vega

### <u>Building sOS</u>
1. Run ``spede-mkmf`` to create a makefile on the SpedeTarget if one does not exist. Change the variable *OS_NAME* to *sOS*<br><br>
2. Either use ``make`` or ``make debug`` to generate the OS image (.dli file)
    * Note: ``make clean`` can be used to remove the files generated by the makefile<br><br>
3. Run ``flash sOS.dli`` followed by ``download`` to load the onto SpedeTarget<br><br>
4. Run ``flint`` followed by `g` to run the OS on the target machine.
    * If you wish to debug the OS run ``gdb`` followed by ``continue`` to debug.
        * Note: best used with an OS image created with ``make debug``

Tagged 4.0

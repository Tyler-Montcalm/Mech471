
changes required for joystick project

1. need to copy afxres.h to local directory from ..\VC2012_atlmfc\VC\atlmfc\include\

2. project properties -> manifest tool -> IO -> Embed manifest -> No

3. Add include and lib directories from SDK directory

4. Need to have some sample files from directory above
-- initially copyied entire C++ example directory from directX samples

5. Need to put the direct.ico into the local directory and change the
reference from a common directory to the local directory -- ie remove 
the path spec

6. For shared memory need to remove unicode config -- select 
project preferences and config manager at top and select Debug.
-> then need to do steps above again.

7. Turn SAFESEH off in linker settings

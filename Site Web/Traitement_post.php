<?php
$datastring = $_POST[];
$myfile = fopen("newfile.txt", "w") or die ("Unable to open file")
$txt = $datastring."\n";
fwrite($myfile,$txt)
fclose($myfile);
echo $txt;
 
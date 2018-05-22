<html>
	<head>
		<title> Test de Capteur récupérer </title>
	</head>
	<body>
		<h1>test</h1>
		<?php
			if (isset($_GET['num_capt']) AND isset($_GET['val_capt'])){
				echo 'Capteur '.$_GET['num_capt'].' : '.$_GET['val_capt'].;
			}
			else{
				echo 'aucun capteur rentré';
			} 
		?>
		<p>
			
				
		</p>
	</body>
	
</html>
<?php
	include 'connect.php';

?>

<!doctype html>
<html class="no-js" lang="">

<head>
  <meta charset="utf-8">
  <title></title>
  <link rel="stylesheet" href="css/normalize.css">
  <link rel="stylesheet" href="css/main.css">
</head>

<body>
  <header>
	
    <div id="logo">
      <img src="img/logo.png" width="200" height="100">

    </div>
  </header>
  <?php
	$sql = "SELECT * FROM data;";
	$result = mysqli_query($conn,  $sql);
	$resultCheck = mysqli_num_rows($result);
	
	if($resultCheck > 0)
	{
		while($row = mysqli_fetch_assoc($result))
		{
			$var = $row['level'];
			if($row['level'] < 50){
				$water_level = true;

			}
			else {
				$water_level = false;
			}
		}
	}
	
	
?>
  <p class="ispis"> Trenutni nivo vode je:  
  <?php echo $var ?></p>
  
  <div class="container">
    <img src="img/slika1.jpg" alt="" width=100% height=100%>
	
	<?php if($water_level == false) : ?>
		<button class="btngreen"> OK </button>
	<?php else : ?>
		<button class="btnred"> DANGEROUS </button>
	<?php endif; ?>

  </div>
  <?php while(1){?>
  <?php } ?>

  
  
</body>

</html>
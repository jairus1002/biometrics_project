<?php
// Database connection parameters
$servername = "localhost"; // Change this to your database server name
$username = "root"; // Change this to your database username
$password = ""; // Change this to your database password
$dbname = "user"; // Change this to your database name

// Connect to the database
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Query to fetch user details from the database
$sql = "SELECT * FROM d_records";

// Execute the query
$result = mysqli_query($conn, $sql);

// Check if any rows were returned
if (mysqli_num_rows($result) > 0) {
    // Array to store user details
    $users = array();

    // Fetch rows from the result set
    while ($row = mysqli_fetch_assoc($result)) {
        // Add each user's details to the array
        $users[] = $row;
    }

    // Return user details as JSON
    header('Content-Type: application/json');
    echo json_encode($users);
} else {
    // No users found
    echo json_encode(["error" => "No users found"]);
}

// Close the database connection
mysqli_close($conn);
?>

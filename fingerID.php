<?php
session_start();

$server_name = "localhost";
$username = "root";
$password = "";
$database_name = "user";

// Connect to MySQL database
$conn = mysqli_connect($server_name, $username, $password, $database_name);

// Check if connection was successful
if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}

// Check if the request method is POST
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Check if the button to generate fingerprint ID was clicked
    if (isset($_POST['generateID'])) {
        // Initialize a flag to indicate if the fingerprint ID exists in the session
        if (!isset($_SESSION['fingerprint_id'])) {
            // If not set, initialize it to 1
            $_SESSION['fingerprint_id'] = 1;
        } else {
            // If set, increment it by 1 (up to a maximum of 127)
            $_SESSION['fingerprint_id'] = min($_SESSION['fingerprint_id'] + 1, 127);
        }
        
        // Return the generated fingerprint ID
        echo $_SESSION['fingerprint_id'];
    } else {
        // Return an error for unsupported request method
        echo json_encode(["status" => "error", "message" => "Invalid request"]);
    }
} elseif ($_SERVER['REQUEST_METHOD'] === 'GET') {
    // Handle other types of requests or return user details

    // Retrieve user details from the registration table
    $sql = "SELECT * FROM registration";

    // Execute the query and fetch data
    $result = mysqli_query($conn, $sql);
    if ($result) {
        $data = array();
        while ($row = mysqli_fetch_assoc($result)) {
            $data[] = $row;
        }

        // Return user details as JSON
        header('Content-Type: application/json');
        echo json_encode($data);
    } else {
        // Error fetching data
        echo json_encode(["error" => "Error fetching user data"]);
    }
} else {
    // Return an error for unsupported request method
    echo json_encode(["status" => "error", "message" => "Unsupported request method."]);
}

// Close the database connection
mysqli_close($conn);
?>

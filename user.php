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
        // Generate a unique fingerprint ID
        $fingerprint_id = mt_rand(1, 127);

        // Get user details from the POST data
        $email = mysqli_real_escape_string($conn, $_POST['email']);
        $first_name = mysqli_real_escape_string($conn, $_POST['first_name']);
        $second_name = mysqli_real_escape_string($conn, $_POST['second_name']);
        $password = password_hash(mysqli_real_escape_string($conn, $_POST['password']), PASSWORD_DEFAULT);
        $department = mysqli_real_escape_string($conn, $_POST['department']);

        // Check if the email already exists in the database
        $sql_check_email = "SELECT * FROM registration WHERE email = '$email'";
        $result_check_email = mysqli_query($conn, $sql_check_email);
        if (mysqli_num_rows($result_check_email) > 0) {
            // Email already exists, retrieve existing fingerprint ID
            $row = mysqli_fetch_assoc($result_check_email);
            $existing_fingerprint_id = $row['fingerprint_status'];
            $fingerprint_id = $existing_fingerprint_id; // Retain existing fingerprint ID
        } else {
            // Email does not exist, insert a new record with all details
            $sql_insert = "INSERT INTO registration (email, first_name, second_name, password, department, fingerprint_status) VALUES ('$email', '$first_name', '$second_name', '$password', '$department', '$fingerprint_id')";
            if (!mysqli_query($conn, $sql_insert)) {
                // Return error message if insertion fails
                echo "Error inserting new user: " . mysqli_error($conn);
                exit;
            }
        }

        // Return the generated or existing fingerprint ID
        echo $fingerprint_id;
    } else {
       
    
        // You can add a delay before redirecting to give the user a chance to see the success message
        echo "<script>
                setTimeout(function() {
                    window.location.href = 'login.html'; // Change the URL to your login page
                }, 3000); // Redirect after 3 seconds (adjust as needed)
              </script>";
    }
    
}

// Close the database connection
mysqli_close($conn);
?>

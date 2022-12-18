import time
import os
import re


# Set the number of seconds between pings
PING_INTERVAL = 5

# Set the name of the file to store the ping requests in
FILE_NAME = "pings_results_p.txt"

# Set the IP address or hostname of the machine to ping
PING_TARGET = "10.100.102.59"

# Keep track of the total round-trip time for all pings
total_rtt = 0

# Keep track of the number of pings
ping_count = 0

# Open the file for writing
with open(FILE_NAME, "w") as file:
    try:
        # Continuously ping the target at the specified interval
        while True:
            # Send a ping request
            response = os.system("ping -c 1 " + PING_TARGET)

            # Check if the ping was successful
            if response == 0:
                # The ping was successful, so get the round-trip time
                rtt = (os.popen("ping -c 1 " + PING_TARGET).read())
                file.write("\n" + str(rtt) + "\n")

                # Use a regular expression to extract the time value from the string
                time_string = re.search(r'time=(\d+\.\d+)', rtt).group(1)
                file.write("\n" + str(time_string) + "\n")

                # Convert the time string to a float
                time_value = float(time_string)

                # Add the round-trip time to the total
                total_rtt += time_value

                # Increment the ping count
                ping_count += 1
            else:
                # The ping was not successful, so write "failed" to the file
                file.write("failed\n")

            # Wait for the specified interval before sending the next ping
            time.sleep(PING_INTERVAL)

    except KeyboardInterrupt:
        # Calculate the average round-trip time and write it to the file
        average_rtt = total_rtt / ping_count
        file.write("Average RTT: " + str(average_rtt))


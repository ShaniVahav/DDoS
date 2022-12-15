import time
import os

# Set the number of seconds between pings
PING_INTERVAL = 5

# Set the name of the file to store the ping requests in
FILE_NAME = "ping_requests.txt"

# Set the IP address or hostname of the machine to ping
PING_TARGET = "8.8.8.8"

# Keep track of the total round-trip time for all pings
total_rtt = 0

# Keep track of the number of pings
ping_count = 0

# Open the file for writing
with open(FILE_NAME, "w") as file:
  # Continuously ping the target at the specified interval
  while True:
    # Send a ping request
    response = os.system("ping -c 1 " + PING_TARGET)

    # Check if the ping was successful
    if response == 0:
      # The ping was successful, so get the round-trip time
      rtt = float(os.popen("ping -c 1 " + PING_TARGET + " | grep 'time=' | cut -d '=' -f 4").read())
      file.write(str(rtt) + "\n")

      # Add the round-trip time to the total
      total_rtt += rtt

      # Increment the ping count
      ping_count += 1
    else:
      # The ping was not successful, so write "failed" to the file
      file.write("failed\n")

    # Wait for the specified interval before sending the next ping
    time.sleep(PING_INTERVAL)

  # Calculate the average round-trip time and write it to the file
  average_rtt = total_rtt / ping_count
  file.write("Average RTT: " + str(average_rtt))
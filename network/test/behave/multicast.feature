Feature: Multicast communication between multiple hosts

  Scenario: Multicast single sender single receiver
    Given Docker set the network behave-test of type bridge
    And Multicast sender application runs on the host 172.0.0.2 port 98989 and mcast group 255.255.255.255
    And Multicast receiver application runs on the host 172.0.0.2 port 98989 and mcast group 255.255.255.255

    When Multicast sender sends a message "hello"
    Then Multicast receiver receives the message "hello"

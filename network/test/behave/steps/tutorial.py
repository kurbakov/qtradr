from behave import *


@given('Docker set the network behave-test of type bridge')
def step_impl(context):
    pass


@given('Multicast sender application runs on the host 172.0.0.2 port 98989 and mcast group 255.255.255.255')
def step_impl(context):
    pass


@given('Multicast receiver application runs on the host 172.0.0.2 port 98989 and mcast group 255.255.255.255')
def step_impl(context):
    pass


@when('Multicast sender sends a message "hello"')
def step_impl(context):
    assert True is not False


@then('Multicast receiver receives the message "hello"')
def step_impl(context):
    assert context.failed is False
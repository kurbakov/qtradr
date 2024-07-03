# Config

Library uses [mINI](https://github.com/metayeti/mINI) to parse config files.

We expect a collection of configs for fully functional program.

Example of the session config file:

```ini
[credentials]
api_key=${API_KEY}
api_secret=${API_SECRET}

[session.static_data]
host=api.coinbase.com
port=443

[session.market_data]
host=advanced-trade-ws.coinbase.com
port=443

[session.trading]
host=api.coinbase.com
port=443
```

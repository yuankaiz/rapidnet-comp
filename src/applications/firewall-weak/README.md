# Firewall Properties

## Safety Invariant 1

### English

The destination of a packet received by an trusted machine must be trusted by the controller.

### Logic

```
forall Host, Port, Src, SrcPort, Switch,
   pktReceived(Host, Port, Src, SrcPort, Switch),
    ->
    exist, trustedControllerMemory(@Controller, Switch, Src).
        
```

Result: True
Time: 40ms

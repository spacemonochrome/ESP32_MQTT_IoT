// See https://aka.ms/new-console-template for more information
using HiveMQtt.Client;
using HiveMQtt.Client.Options;

var options = new HiveMQClientOptions();
options.Host = "1138962d42f345e094875d27a43a0e78.s1.eu.hivemq.cloud";
options.Port = 8883;
options.UserName = "hasanfiratkeskin";
options.Password = "abdulselam12";
options.UseTLS = true;

var client = new HiveMQClient(options);

var connectResult = await client.ConnectAsync().ConfigureAwait(false);

await client.PublishAsync(
                "ilk_mesaj", // Topic to publish to
                "ahmed muhsin"                    // Message to publish
                ).ConfigureAwait(false);


client.OnMessageReceived += (sender, args) =>
{
    // Handle Message in args.PublishMessage
    Console.WriteLine("Message Received: {}", args.PublishMessage.PayloadAsString);
};
await client.SubscribeAsync("ilk_mesaj").ConfigureAwait(true);


Console.ReadKey();




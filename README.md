# Sentinel Guard
> Automatically enable and disable security measures against EAC.

Very simple program to automatically configure process isolation and process tracing to minimize exposure of root owned processes to less privileged processes.
I wrote this to make it easier for me to disable my protection in order to let EAC initialize correctly but then enable it again when I want to load my fork of [zap-client](https://github.com/hir0xygen/zap-client).
I'm looking into figuring out how to fully run Steam and Apex in a container as well, if I ever do it will be added to Sentinel Guard.

## Installation

Dependencies:

```sh
pacman -S boost
```

Building:

```sh
git clone https://github.com/hir0xygen/sentinelguard
```
```sh
cd sentinelguard
```
```sh
./cleanbuild.sh
```
```sh
./sentinelguard --help
```

## Usage example

Running the application without arguments or with the `--help` arguments lists options.
1. Make sure protection is disabled: `./sentinelguard --disable`
2. Launch Apex Legends, wait until EAC initialization is completed
3. Enable protection `./sentinelguard --enable`
4. Load your cheat

## Release History

* 0.0.1
    * Automatically toggle process isolation and process tracing

## Meta

Distributed under the MIT license. See ``LICENSE`` for more information.

[https://github.com/hir0xygen/sentinelguard](https://github.com/hir0xygen)

## Contributing

1. Fork it (<https://github.com/hir0xygen/sentinelguard/fork>)
2. Create your feature branch (`git checkout -b feature/fooBar`)
3. Commit your changes (`git commit -am 'Add some fooBar'`)
4. Push to the branch (`git push origin feature/fooBar`)
5. Create a new Pull Request
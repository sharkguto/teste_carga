# select image
FROM rust:latest as build

# create a new empty shell project
RUN USER=root cargo new --bin conderp-rust-backend
WORKDIR /opt/conderp

# copy over your manifests
COPY ./Cargo.lock ./Cargo.lock
COPY ./Cargo.toml ./Cargo.toml

# this build step will cache your dependencies
RUN cargo build --release || true

# copy your source tree
COPY ./src ./src

# build for release
#RUN rm ./target/release/deps/conderp*
RUN RUSTFLAGS="-C target-cpu=native" cargo build --release

FROM debian:stretch

WORKDIR /opt/conderp
COPY --from=build /opt/conderp/target/release/conderp .
EXPOSE 8080
RUN chmod 775 conderp
# set the startup command to run your binary
CMD ["/opt/conderp/conderp"]

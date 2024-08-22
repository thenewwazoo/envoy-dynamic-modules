# contents lifted from
# https://github.com/envoyproxy/envoy/blob/main/.devcontainer/Dockerfile
FROM envoyproxy/envoy-build-ubuntu:75238004b0fcfd8a7f71d380d7a774dda5c39622 as build

ARG USERNAME=envoybuild
ARG USER_UID=501
ARG USER_GID=$USER_UID

ENV BUILD_DIR=/build

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get -y update \
  # install build-essential to give us cc(1) until I make the following .cargo/config.toml file work
  #
  #   [target.x86_64-unknown-linux-gnu]
  #   rustflags = ["-C", "linker=clang", "-C", "link-arg=-fuse-ld=lld"]
  && apt-get -y install --no-install-recommends build-essential 2>&1 \
  # Create a non-root user to use if preferred - see https://aka.ms/vscode-remote/containers/non-root-user.
  && groupadd --gid $USER_GID $USERNAME \
  && useradd -s /bin/bash --uid $USER_UID --gid $USER_GID -m $USERNAME -d /build \
  # [Optional] Add sudo support for non-root user
  && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
  && chmod 0440 /etc/sudoers.d/$USERNAME

WORKDIR /build/
COPY . .
RUN chown -R $USERNAME:$USERNAME .

# ENV BAZELRC_FILE=/build/.bazelrc
RUN ./bazel/setup_clang.sh /opt/llvm/
#RUN echo "build --config=rbe-toolchain-clang-libc++" >> ~/.bazelrc
#RUN echo "build ${BAZEL_BUILD_EXTRA_OPTIONS}" | tee -a ~/.bazelrc

ENV CC=/opt/llvm/bin/clang
ENV PATH=/opt/llvm/bin/:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

USER envoybuild
# cache a layer to build envoy
FROM build AS warm
ARG LEVEL=dbg
RUN bazel build -c $LEVEL @envoy//source/exe:envoy_main_entry_lib
# now build the extensions to link in
FROM warm as binary
ARG LEVEL=dbg
RUN bazel build -c $LEVEL envoy

FROM envoyproxy/envoy:dev AS runtime
COPY --from=binary /build/bazel-bin/envoy /usr/local/bin/envoy

CMD ["/bin/envoy", "-c", "/etc/envoy/envoy.yaml"]

FROM runtime
